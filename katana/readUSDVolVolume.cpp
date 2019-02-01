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
#include "readUSDVolVolume.h"

#include <usdKatana/attrMap.h>
#include <usdKatana/readPrim.h>
#include <usdKatana/readXformable.h>

#include <pxr/usd/usdVol/openVDBAsset.h>
#include <pxr/usd/usdVol/volume.h>

#include "arnoldHelpers.h"

#include <FnAttribute/FnDataBuilder.h>

PXR_NAMESPACE_OPEN_SCOPE

void readUSDVolVolume(
    FnKat::GeolibCookInterface& interface, FnKat::GroupAttribute opArgs,
    const PxrUsdKatanaUsdInPrivateData& privateData) {
    const auto prim = privateData.GetUsdPrim();
    const auto volume = UsdVolVolume(prim);
    const auto currentTime = privateData.GetUsdInArgs()->GetCurrentTime();

    const auto fieldPathMap = volume.GetFieldPaths();
    if (fieldPathMap.empty()) {
        ReportError(interface, "UsdVolVolume has no field relationships");
        return;
    }

    const auto stagePtr = prim.GetStage();
    const auto volumePath = prim.GetPath();

    std::unordered_set<std::string> vdbPaths;
    std::vector<std::string> vdbFieldNames;
    FnKat::GroupBuilder skipChildren;

    SdfAssetPath filePath;
    TfToken fieldName;
    for (const auto& fieldPair : fieldPathMap) {
        const UsdPrim& fieldPrim = stagePtr->GetPrimAtPath(fieldPair.second);
        if (!fieldPrim) { continue; }
        const auto vdbField = UsdVolOpenVDBAsset(fieldPrim);
        if (!vdbField) { continue; }
        if (fieldPair.second.GetParentPath() == volumePath) {
            skipChildren.set(
                fieldPrim.GetName().GetText(), FnKat::IntAttribute(1));
        }
        vdbField.GetFilePathAttr().Get<SdfAssetPath>(&filePath, currentTime);
        if (vdbPaths.insert(filePath.GetResolvedPath()).second &&
            vdbPaths.size() > 1) {
            ReportError(
                interface,
                "UsdVolVolume has VDB fields with differing asset paths");
            return;
        }
        // Trust the field name attribute on the VDB asset over the relationship
        // name. Note that Houdini 17 currently writes time samples for this
        // attribute, so we just use the first available sample in that case.
        if (!vdbField.GetFieldNameAttr().Get<TfToken>(&fieldName)) {
            vdbField.GetFieldNameAttr().Get<TfToken>(
                &fieldName, UsdTimeCode::EarliestTime());
        }
        vdbFieldNames.push_back(fieldName.GetString());
    }

    interface.setAttr("type", FnKat::StringAttribute("volume"));

    if (vdbPaths.size() != 1) {
        TF_WARN(
            "UsdVolVolume %s has no VDB asset fields. No Arnold volume "
            "will be configured.",
            volumePath.GetText());
        return;
    }

    // Read in the generic prim attributes. This handles things like material
    // binding, visibility, primvars, etc.
    PxrUsdKatanaAttrMap attrs;
    PxrUsdKatanaReadPrim(prim, privateData, attrs);
    attrs.toInterface(interface);

    // Advertise the connected field names
    interface.setAttr(
        "info.usdAi.vdbFields", FnKat::StringAttribute(vdbFieldNames));

    // If any of the VDB field prims are children of the Volume prim (which is
    // the recommended organization), tell the `pxrUsdIn` op to skip over them,
    // to avoid generating extra empty groups that may cause confusion.
    if (skipChildren.isValid()) {
        interface.setAttr("__UsdIn.skipChild", skipChildren.build());
    }

    // Build up the arguments required to execute an ArnoldOpenVDBVolume op.
    FnKat::GroupBuilder argsBuilder;
    getArnoldVDBVolumeOpArgs(prim, argsBuilder);
    argsBuilder.set("filename", FnKat::StringAttribute(*vdbPaths.begin()));

    FnKat::GroupAttribute xform;
    if (PxrUsdKatanaReadXformable(volume, privateData, xform)) {
        argsBuilder.set("xform", xform);
    }

    // Forward the time slice information to the Arnold op so the motion range
    // gets set up properly on the volume.
    FnKat::GroupAttribute timeSlice = interface.getOpArg("system.timeSlice");
    ;
    argsBuilder.set(
        "system", FnKat::GroupAttribute("timeSlice", timeSlice, false), false);

    interface.execOp("ArnoldOpenVDBVolume", argsBuilder.build());
}

PXR_NAMESPACE_CLOSE_SCOPE
