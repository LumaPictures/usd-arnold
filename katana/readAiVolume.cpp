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
#include "readAiVolume.h"

#include <usdKatana/attrMap.h>
#include <usdKatana/readPrim.h>
#include <usdKatana/readXformable.h>

#include <pxr/usd/usdAi/aiVolume.h>

#include "arnoldHelpers.h"

#include <FnAttribute/FnDataBuilder.h>

PXR_NAMESPACE_OPEN_SCOPE

void readAiVolume(
    FnKat::GeolibCookInterface& interface, FnKat::GroupAttribute opArgs,
    const PxrUsdKatanaUsdInPrivateData& privateData) {
    const auto prim = privateData.GetUsdPrim();
    const auto volume = UsdAiVolume(prim);
    const auto currentTime = privateData.GetUsdInArgs()->GetCurrentTime();

    // Read in the generic prim attributes. This handles things like material
    // binding, visibility, primvars, etc.
    PxrUsdKatanaAttrMap attrs;
    PxrUsdKatanaReadPrim(prim, privateData, attrs);
    attrs.toInterface(interface);

    // Build up the arguments required to execute an ArnoldOpenVDBVolume op.
    FnKat::GroupBuilder argsBuilder;
    getArnoldVDBVolumeOpArgs(prim, argsBuilder);

    SdfAssetPath filename;
    volume.GetFilenameAttr().Get(&filename, currentTime);
    argsBuilder.set(
        "filename", FnKat::StringAttribute(filename.GetResolvedPath()));

    float stepSize = 0;
    if (UsdAttribute stepAttr = volume.GetStepSizeAttr()) {
        stepAttr.Get<float>(&stepSize, currentTime);
    }
    argsBuilder.set("step_size", FnKat::FloatAttribute(stepSize));

    // Forward the time slice information to the Arnold op so the motion range
    // gets set up properly on the volume.
    FnKat::GroupAttribute timeSlice = interface.getOpArg("system.timeSlice");
    ;
    argsBuilder.set(
        "system", FnKat::GroupAttribute("timeSlice", timeSlice, false), false);

    FnKat::GroupAttribute xform;
    if (PxrUsdKatanaReadXformable(volume, privateData, xform)) {
        argsBuilder.set("xform", xform);
    }
    interface.execOp("ArnoldOpenVDBVolume", argsBuilder.build());
}

PXR_NAMESPACE_CLOSE_SCOPE
