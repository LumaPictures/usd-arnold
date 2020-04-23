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
                  const UsdAiMaterialAPI& material)
{
    FnKat::GroupAttribute nodesAttr = interface.getOutputAttr("material.nodes");
    if (!nodesAttr.isValid()) {
        return;
    }

    const UsdRelationship& surfaceRel = material.GetSurfaceRel();
    const UsdRelationship& dispRel = material.GetDisplacementRel();
    if (!(surfaceRel.HasAuthoredTargets() || dispRel.HasAuthoredTargets())) {
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
        auto it = uniquifiedShaderHandles.find(shader.GetPath());
        if (it != uniquifiedShaderHandles.end()) {
            return it->second;
        }
        return PxrUsdKatanaUtils::GenerateShadingNodeHandle(shader);
    };


    auto mapRelations = [&stage](const UsdRelationship& relationship,
                                 std::function<void(const UsdPrim&)> fn) 
    {
        static thread_local SdfPathVector targets;
        targets.clear();
        relationship.GetTargets(&targets);
        for (const auto& target : targets) {
            const auto shader = stage->GetPrimAtPath(target);
            if (shader.IsValid()) { fn(shader); }
        }
    };

    // It's hard to decide the exact frequency of inserts
    // and reads, but most likely it's the same magnitude.
    // We could also try a vector here.
    std::unordered_set<std::string> processedShaders;

    // We can't use auto here, otherwise the lambda won't be able to capture
    // itself.
    std::function<void(const UsdPrim&)> traverseShader = [&](const UsdPrim&
                                                                 shader) {
        // TODO: we can also use getInputs from the new API.
        const auto shadingNodeHandle = getShaderHandle(shader);
        auto insertResult = processedShaders.insert(shadingNodeHandle);
        if (!insertResult.second) {
            return;
        }

        if (!nodesAttr.getChildByName(shadingNodeHandle).isValid()) {
            return;
        }

        FnKat::GroupBuilder builder;
        for (const auto& relationship : shader.GetRelationships()) {
            static const std::string connectedSourceFor("connectedSourceFor:");
            const auto relationshipName = relationship.GetName().GetString();
            if (relationshipName.compare(
                    0, connectedSourceFor.length(), connectedSourceFor) != 0) {
                continue;
            }

            auto paramName =
                relationshipName.substr(connectedSourceFor.length());
            auto colonPos = paramName.find(':');
            if (colonPos != paramName.npos) { // either array elem or component
                auto comp = paramName.substr(colonPos + 1);
                paramName = paramName.substr(0, colonPos);
                if (comp.length() == 0) {
                    continue;
                } // Just to make sure it's not a malformed
                // variable, like one that ends with a :
                if (comp[0] == 'i') { // array connection
                    paramName += ":" + comp.substr(1);
                } else {
                    paramName += "." + comp;
                }
            } else {
                continue; // The existing code already handles this!
            }

            static thread_local SdfPathVector targets;
            targets.clear();
            relationship.GetTargets(&targets);
            if (targets.size() != 1) { continue; }

            const auto& target = targets.front();
            auto targetName = target.GetName();
            static const std::string outputs("outputs:");
            if (targetName.compare(0, outputs.length(), outputs) != 0) {
                continue;
            }
            targetName = targetName.substr(outputs.length());
            if (targetName != "out") { // component connection
                targetName = "out." + targetName;
            }
            const auto targetPrim =
                stage->GetPrimAtPath(target.GetParentPath());
            const auto targetHandle = getShaderHandle(targetPrim);
            std::stringstream targetSS;
            targetSS << targetName << '@';
            targetSS << targetHandle;
            builder.set(paramName, FnKat::StringAttribute(targetSS.str()));

            // TODO: we might traverse things twice because of this.
            // Also, infinite recursion, beware!
            mapRelations(relationship, traverseShader);
        }

        using param_split_t = std::vector<std::string>;
        auto splitParamName = [](const std::string& name,
                                 param_split_t& out) -> size_t {
            out.clear();
            size_t currentIndex = 0;
            const auto lastPos = name.size() - 1;
            while (true) {
                size_t colonPos = name.find(':', currentIndex);
                out.push_back(
                    name.substr(currentIndex, colonPos - currentIndex));
                if (colonPos == name.npos || colonPos == lastPos) { break; }
                currentIndex = colonPos + 1;
            }
            return out.size();
        };
        // Per array connections already work with the new API.
        // It seems we can't do full connections and component connections at
        // the same time. To support both combined (full connections and partial
        // connections we have to collect which full connections are explicitly
        // set by the user, and which partial ones. After that we need to
        // manually connect the rest of the component connections not covered by
        // the user.
        UsdShadeConnectableAPI connectableAPI(shader);
        if (connectableAPI) {
            // TODO: We really need to use more optimal data storage here
            std::set<std::string> fullConnections;
            std::map<std::string, std::set<std::string>> partialConnections;
            for (const auto& input : connectableAPI.GetInputs()) {
                if (input.HasConnectedSource()) {
                    const auto inParamName = input.GetBaseName().GetString();
                    SdfPathVector sourcePaths;
                    input.GetRawConnectedSourcePaths(&sourcePaths);
                    if (sourcePaths.empty()) { continue; }
                    // We are only checking for the first connection
                    const auto& sourceParamPath = sourcePaths[0];
                    const auto sourcePath = sourceParamPath.GetPrimPath();
                    const auto sourcePrim = stage->GetPrimAtPath(sourcePath);
                    if (sourcePrim.IsValid()) {
                        traverseShader(sourcePrim);
                    } else {
                        continue;
                    }

                    const auto sourcePrimHandle = getShaderHandle(sourcePrim);
                    static thread_local param_split_t _targetParamSplit;
                    const auto _targetSplitCount =
                        splitParamName(inParamName, _targetParamSplit);
                    if (_targetSplitCount >= 3) {
                        // Connection to Array elements, no idea how to handle
                        // this. Yet.
                        continue;
                    }

                    // Connections to array elements are already handled.
                    // Or we are having an invalid string.
                    // But we still need to handle cases when a source component
                    // is connected to a target tuple. The source param to
                    // target param is done properly by pxrUsdIn.
                    if (_targetSplitCount == 2 &&
                        (_targetParamSplit[1].empty() ||
                         _targetParamSplit[1].front() == 'i')) {
                        continue;
                    }

                    const auto targetParamName =
                        _targetSplitCount == 1
                            ? _targetParamSplit[0]
                            : _targetParamSplit[0] + "." + _targetParamSplit[1];

                    const auto& sourceParam = sourceParamPath.GetName();
                    static thread_local param_split_t _sourceParamSplit;
                    const auto sourceSplitCount =
                        splitParamName(sourceParam, _sourceParamSplit);
                    if (sourceSplitCount != 2) {
                        continue;
                    } // we only support component connections for now
                    // this is both covers out and components
                    if ((_sourceParamSplit[1].empty() ||
                         _sourceParamSplit[1].front() == 'i')) {
                        continue;
                    }
                    if (_targetSplitCount == 2) {
                        partialConnections[_targetParamSplit[0]].insert(
                            _targetParamSplit[1]);
                    }
                    const auto sourceParamAndComponentName =
                        (_sourceParamSplit[1] == "out"
                             ? "out@"
                             : ("out." + _sourceParamSplit[1] + "@")) +
                        sourcePrimHandle;

                    builder.set(
                        targetParamName,
                        FnKat::StringAttribute(sourceParamAndComponentName));
                }
            }
            for (auto it = partialConnections.cbegin();
                 it != partialConnections.cend(); ++it) {
                if (fullConnections.find(it->first) == fullConnections.end()) {
                    // We don't have to do anything, the full connection will be
                    // replaced.
                    continue;
                }
            }
        }

        FnKat::Attribute connections =
            builder.isValid() ? builder.build() : FnKat::Attribute();
        static const std::string baseAttr("material.nodes.");
        std::stringstream ss;
        ss << baseAttr << shadingNodeHandle << ".connections";
        updateOrCreateAttr(interface, ss.str(), connections);
    };

    mapRelations(surfaceRel, traverseShader);
    mapRelations(dispRel, traverseShader);
}

PXR_NAMESPACE_CLOSE_SCOPE
