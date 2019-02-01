// Copyright (c) 2019 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "readPrim.h"

#include <pxr/base/tf/type.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdAi/aiAOV.h>
#include <pxr/usd/usdAi/aiMaterialAPI.h>
#include <pxr/usd/usdAi/aiShapeAPI.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/shader.h>

#include <usdKatana/attrMap.h>
#include <usdKatana/utils.h>

#include "arnoldHelpers.h"
#include "readAOV.h"
#include "readMaterial.h"

PXR_NAMESPACE_OPEN_SCOPE

// TODO: Implement AttributeKeyedCache subclass to reuse computed AOV data, key
// it off of some subset of the op args (and possibly graph state, if needed).
static void resolveAOVs(
    FnKat::GeolibCookInterface& interface, const UsdPrim& rootPrim) {
    // This could just be `static TfToken aovPrimType("AiAOV");`, but I wanted
    // to use something that would insulate against class name changes better.
    // The tradeoff is that this will crash if `UsdAiAOV` has no aliases... :D
    static TfToken aovPrimType(TfType::Find<UsdSchemaBase>()
                                   .GetAliases(TfType::Find<UsdAiAOV>())
                                   .front());

    std::vector<UsdPrim> aovPrims;
    auto aovPredicate = [&](const UsdPrim& prim) {
        return prim.GetTypeName() == aovPrimType;
    };

    const UsdPrimRange rootRange(rootPrim);
    std::copy_if(
        rootRange.begin(), rootRange.end(), std::back_inserter(aovPrims),
        aovPredicate);

    // Note: This only really makes sense if `rootPrim` is the stage root.
    for (auto& masterPrim : rootPrim.GetStage()->GetMasters()) {
        const UsdPrimRange range(masterPrim);
        std::copy_if(
            range.begin(), range.end(), std::back_inserter(aovPrims),
            aovPredicate);
    }

    if (aovPrims.empty()) { return; }

    std::unordered_set<std::string> aovNames;

    // Reusable builders
    FnKat::GroupBuilder channelBuilder(FnKat::GroupBuilder::BuilderModeStrict);
    channelBuilder.reserve(6);
    FnKat::GroupBuilder driverBuilder(FnKat::GroupBuilder::BuilderModeStrict);
    driverBuilder.reserve(4);

    // NOTE: Eventually, we'll probably end up phasing out the op registered
    // for AiAOV prims. When that happens, we should be able to refactor/remove
    // the shared helper functions `getAOVRelationshipTarget` and
    // `readAOVChildShaderPrim` (in `readAOV.cpp`), and probably optimize the
    // code below that uses them a bit more.

    for (auto aovPrim : aovPrims) {
        const UsdAiAOV aov(aovPrim);
        if (!aov) { continue; }

        std::string name;
        if (!aov.GetNameAttr().Get<std::string>(&name)) { continue; }

        std::pair<decltype(aovNames)::iterator, bool> insertResult =
            aovNames.insert(name);
        if (!insertResult.second) { continue; }

        // TODO: Should we allow these through and just use our defaults?
        const UsdShadeShader driver =
            getAOVRelationshipTarget(aov.GetDriverRel());
        if (!driver) {
            TF_WARN(
                "Invalid driver relationship target on AiAOV %s",
                aovPrim.GetPath().GetText());
            continue;
        }
        const UsdShadeShader filter =
            getAOVRelationshipTarget(aov.GetFilterRel());
        if (!filter) {
            TF_WARN(
                "Invalid filter relationship target on AiAOV %s",
                aovPrim.GetPath().GetText());
            continue;
        }

        // Driver params
        static TfToken driver_exr("driver_exr");
        static TfToken driver_deepexr("driver_deepexr");

        TfToken driverId;
        driver.GetIdAttr().Get(&driverId);
        if (driverId == driver_exr) {
            static FnKat::GroupAttribute exrDriverTemplate(
                "compression", FnKat::StringAttribute("zips"), "half_precision",
                FnKat::IntAttribute(1), "tiled", FnKat::IntAttribute(0),
                "autocrop", FnKat::IntAttribute(1), false);
            driverBuilder.update(exrDriverTemplate);
        } else if (driverId == driver_deepexr) {
            driverBuilder.set("tiled", FnKat::IntAttribute(0));
        }
        readAOVChildShaderPrim(driver, driverBuilder);

        // Filter params
        FnKat::GroupBuilder filterBuilder(
            FnKat::GroupBuilder::BuilderModeStrict);
        TfToken filterId = readAOVChildShaderPrim(filter, filterBuilder);

        // Build output channel group
        const FnKat::StringAttribute nameAttr(name);
        TfToken dataType;
        aov.GetDataTypeAttr().Get(&dataType);

        // TOOD: Once the AiAOV schema has a "channel" attribute, we should use
        // it for the "channel" value here.
        channelBuilder.set("name", nameAttr)
            .set("channel", nameAttr)
            .set("driver", FnKat::StringAttribute(driverId.GetText()))
            .set("filter", FnKat::StringAttribute(filterId.GetText()))
            .set("type", FnKat::StringAttribute(dataType.GetText()))
            .set("driverParameters", driverBuilder.build());

        if (filterBuilder.isValid()) {
            channelBuilder.set("filterParameters", filterBuilder.build());
        }

        std::string lightPathExpression;
        if (aov.GetLPEAttr().Get<std::string>(&lightPathExpression)) {
            channelBuilder.set(
                "lightPathExpression",
                FnKat::StringAttribute(lightPathExpression));
        }

        static const std::string channelAttrPrefix(
            "arnoldGlobalStatements.outputChannels.");
        interface.setAttr(
            channelAttrPrefix + name, channelBuilder.build(), false);

        // Build render output group
        // TODO: I don't really like hardcoding the `locationType` in here, but
        // I'm initially just preserving the workflow status quo.
        static const std::string renderOutputPrefix("renderSettings.outputs.");
        interface.setAttr(
            renderOutputPrefix + name,
            FnKat::GroupAttribute(
                "rendererSettings",
                FnKat::GroupAttribute("channel", nameAttr, false),
                "locationType", FnKat::StringAttribute("file"), false),
            false);
    }
}

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
void readPrimLocation(
    FnKat::GeolibCookInterface& interface, FnKat::GroupAttribute opArgs,
    const PxrUsdKatanaUsdInPrivateData& privateData) {
    const auto prim = privateData.GetUsdPrim();
    // As of 0.8.2 this function is also called on the root: "/"
    if (!prim.IsValid() || !prim.GetPrimPath().IsAbsoluteRootOrPrimPath()) {
        return;
    }

    if (prim.GetPrimPath() == SdfPath::AbsoluteRootPath()) {
        resolveAOVs(interface, prim);
        return;
    }

    auto stage = prim.GetStage();

    static const std::string statementsName("arnoldStatements");
    updateOrCreateAttr(
        interface, statementsName, getArnoldStatementsGroup(prim));

    // We are handling connections here, because usd-arnold stores the
    // connections in it's own way. So we check for the materials connected to
    // the node.
    // TODO: Shouldn't we just register our own op to handle the extra material
    // connection requirements?
    const UsdShadeMaterial material(prim);
    if (material) {
        UsdAiMaterialAPI aiMaterialAPI(prim);
        readMaterial(stage, interface, aiMaterialAPI);
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
