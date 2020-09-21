// Copyright 2019 Luma Pictures
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "readMaterial.h"

#include <pxr/usd/usdShade/connectableAPI.h>

#include <pxr/usd/usdAi/aiMaterialAPI.h>

#include <usdKatana/utils.h>

#include "arnoldHelpers.h"


PXR_NAMESPACE_OPEN_SCOPE


/*
    There are a couple of things that are important to note when
    handling connections in Katana. We let USD import its connections
    but we are overwriting it using the read prim location function
    to make sure all the component connections work fine that we support
    in usd-arnold.

    When USD imports a shading network, it's creating a network material
    for each network material (so certain nodes might get duplicated). Since
    the network material in Katana uses strings to identify each material,
    we need to use a remapping function to figure out the names for the
   materials. This is PxrUsdKatanaUtils::GenerateShadingNodeHandle . Without
   this function you won't get the right shader names in some cases.

    Katana creates a group attribute that stores all the nodes, named
   material.nodes, which has all node definitions. Each node has multiple
   parameters, the most important for us is the connections group attribute.
   This stores the incoming connections to each node. Each sub attribute here is
   either a String Attribute, or a Group Attribute storing multiple String
   Attributes.

    In case of a connection where the target is the full parameter (ie, none of
   it's components) you can setup the connection by creating a string attribute
   like
    ("TargetParamName", "SourceParamName")

    If you have a connection that's targeting a parameter's connection, you need
   to create a group attribute named like the parameter, and a string attribute
   with the component's name. Or just create a String Attribute like
   parameter.component, and that'll create the group for you. It looks like
   this:
    ("TargetParamName.TargetComponentName", "SourceParamName")

    Component names are simply r, g, b, a, x, y, z in case of colors and
   vectors, otherwise you need to use the i prefix for array elements, like i0,
   i1, i2.

    Source connection can also represent a parameter or a parameter's component.
   In the case of a parameter, you need to name the source parameter the
   following way: out@SourceNodeName

    If you are connecting a parameter's component, you need to name the source
   param like this: out.componentName@SourceNodeName

    Like: out.r@SourceNodeName, out.x@SourceNodeName

    A couple of example String Attributes that represent a valid connection.

    ("kd_Color", "out@MyTexture")
    ("inputs:i1", "out@Checker")
    ("kd_Color.r", "out.g@MyTexture")
*/


void readMaterial(UsdStageWeakPtr stage, FnKat::GeolibCookInterface& interface,
                  const UsdShadeMaterial& material)
{
    FnKat::GroupAttribute nodesAttr = interface.getOutputAttr("material.nodes");
    if (!nodesAttr.isValid()) {
        return;
    }

    // Collect shader prims that are connected to either Arnold UsdShadeOutputs
    // or UsdAi relationships on the material.
    static const TfToken _arnold("arnold");
    std::vector<UsdPrim> materialOutputPrims;
    {
        // UsdShade outputs in "arnold:" namespace
        UsdShadeConnectableAPI source;
        TfToken srcName;
        UsdShadeAttributeType srcType;
        for (auto outputGetter : {&UsdShadeMaterial::GetSurfaceOutput,
                                  &UsdShadeMaterial::GetDisplacementOutput,
                                  &UsdShadeMaterial::GetVolumeOutput})
        {
            if (UsdShadeOutput output = ((material).*(outputGetter))(_arnold)) {
                if (output.GetConnectedSource(&source, &srcName, &srcType)) {
                    materialOutputPrims.emplace_back(source.GetPrim());
                }
            }
        }

        // UsdAi material relationships
        const UsdAiMaterialAPI aiMaterial(material.GetPrim());
        SdfPathVector targets;
        for (auto relationshipGetter : {&UsdAiMaterialAPI::GetSurfaceRel,
                                        &UsdAiMaterialAPI::GetDisplacementRel,
                                        &UsdAiMaterialAPI::GetVolumeRel})
        {
            if (UsdRelationship rel = ((aiMaterial).*(relationshipGetter))()) {
                targets.clear();
                if (rel.GetForwardedTargets(&targets) && targets.size() > 0) {
                    // We only care about the first target.
                    materialOutputPrims.emplace_back(
                        stage->GetPrimAtPath(targets[0]));
                }
            }
        }
    }

    if (materialOutputPrims.empty()) {
        return;
    }

    // Build a sparse map of shader prims whose handles have been mangled by
    // usdKatana to preserve uniqueness.
    std::unordered_map<SdfPath, std::string, SdfPath::Hash> uniquifiedShaderHandles;
    for (int64_t i = 0; i < nodesAttr.getNumberOfChildren(); i++)
    {
        FnKat::GroupAttribute nodeGroup = nodesAttr.getChildByIndex(i);
        // This attr will only be set if the handle has been uniquified.
        FnKat::StringAttribute primPathAttr = 
            nodeGroup.getChildByName("usdPrimPath");
        if (primPathAttr.isValid()) {
            FnKat::StringAttribute nameAttr = nodeGroup.getChildByName("name");
            uniquifiedShaderHandles.emplace(
                SdfPath(primPathAttr.getValue("", false)),
                nameAttr.getValue("", false)
            );
        }
    }


    auto getShaderHandle = [&uniquifiedShaderHandles](const UsdPrim& shader) -> std::string
    {
        const SdfPath primPath = shader.GetPath();
        auto it = uniquifiedShaderHandles.find(primPath);
        if (it != uniquifiedShaderHandles.end()) {
            return it->second;
        }
        const std::string handle = PxrUsdKatanaUtils::GenerateShadingNodeHandle(shader);
        uniquifiedShaderHandles.emplace(primPath, handle);
        return handle;
    };


    std::unordered_set<std::string> processedShaders;

    // We can't use auto here, otherwise the lambda won't be able to capture
    // itself.
    std::function<void(const UsdPrim&)> traverseShader = [&](const UsdPrim& shader) {
        const std::string shadingNodeHandle = getShaderHandle(shader);
        auto insertResult = processedShaders.insert(shadingNodeHandle);
        if (!insertResult.second) {
            return;
        }

        if (!nodesAttr.getChildByName(shadingNodeHandle).isValid()) {
            return;
        }

        using StringVector = std::vector<std::string>;
        auto splitParamName = [](const std::string& name,
                                 StringVector& out) -> size_t {
            out.clear();
            size_t currentIndex = 0;
            const auto lastPos = name.size() - 1;
            while (true) {
                size_t colonPos = name.find(':', currentIndex);
                out.push_back(
                    name.substr(currentIndex, colonPos - currentIndex));
                if (colonPos == std::string::npos || colonPos == lastPos) { break; }
                currentIndex = colonPos + 1;
            }
            return out.size();
        };

        FnKat::GroupBuilder builder;

        // Per array connections already work with the new API.
        // It seems we can't do full connections and component connections at
        // the same time. To support both combined (full connections and partial
        // connections we have to collect which full connections are explicitly
        // set by the user, and which partial ones. After that we need to
        // manually connect the rest of the component connections not covered by
        // the user.
        UsdShadeConnectableAPI connectableAPI(shader);
        if (!connectableAPI) {
            return;
        }

        UsdShadeConnectableAPI source;
        TfToken sourceName;
        UsdShadeAttributeType sourceType;
        for (const auto& input : connectableAPI.GetInputs()) {
            if (!input.GetConnectedSource(&source, &sourceName, &sourceType)) {
                continue;
            }
            if (sourceType == UsdShadeAttributeType::Invalid) {
                continue;
            }

            const UsdPrim sourcePrim = source.GetPrim();
            if (sourcePrim.IsValid()) {
                traverseShader(sourcePrim);
            }
            else {
                continue;
            }

            static thread_local StringVector _targetParamSplit;
            const std::string inParamName = input.GetBaseName().GetString();
            const size_t targetSplitCount =
                splitParamName(inParamName, _targetParamSplit);
            if (targetSplitCount >= 3) {
                // Connection to Array elements, no idea how to handle
                // this. Yet.
                continue;
            }

            // Connections to array elements are already handled.
            // Or we are having an invalid string.
            // But we still need to handle cases when a source component
            // is connected to a target tuple. The source param to
            // target param is done properly by pxrUsdIn.
            if (targetSplitCount == 2 &&
                    (_targetParamSplit[1].empty() ||
                     _targetParamSplit[1].front() == 'i')) {
                continue;
            }

            const std::string& sourceNameString = sourceName.GetString();
            if (sourceNameString.empty() ||
                    sourceNameString.front() == 'i' ||
                    sourceNameString == "out") {
                continue;
            }

            // At this point we know we need to modify the connection source.
            const std::string sourceShaderHandle = getShaderHandle(sourcePrim);
            std::string connectionSource;
            connectionSource.reserve(sourceShaderHandle.size() + 6);

            static const std::string _validComponents("rgbaxyzw");
            if (sourceNameString.size() == 1 &&
                    _validComponents.find(
                        sourceNameString.front()) != std::string::npos) {
                // The connected input corresponds to a recognized Arnold shader
                // component (e.g. "outputs:r"), so append the component name to
                // the Katana connection spec.
                connectionSource.append("out.");
                connectionSource.push_back(sourceNameString.front());
                connectionSource.push_back('@');
            }
            else {
                // Any other name just gets dropped in favor of the default
                // "out" spec, in order to avoid confusing KtoA and/or anyone
                // inspecting the connection attributes. This is mostly in
                // response to the weird naming coming from the HtoA LOPs shader
                // translator, which names each shader's primary output using
                // its datatype (e.g. the primary output from a `float_to_rgb`
                // shader gets named "outputs:rgb").
                connectionSource.append("out@");
            }
            connectionSource.append(sourceShaderHandle);

            const std::string targetParamName =
                targetSplitCount == 1
                    ? _targetParamSplit[0]
                    : _targetParamSplit[0] + "." + _targetParamSplit[1];

            builder.set(targetParamName,
                        FnKat::StringAttribute(connectionSource));
        }

        if (builder.isValid()) {
            std::string attrName;
            attrName.reserve(shadingNodeHandle.size() + 27);
            attrName.append("material.nodes.");
            attrName.append(shadingNodeHandle);
            attrName.append(".connections");
            updateOrCreateAttr(interface, attrName, builder.build());
        }
    };

    for (const UsdPrim& shaderPrim : materialOutputPrims) {
        if (shaderPrim.IsValid()) {
            traverseShader(shaderPrim);
        }
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
