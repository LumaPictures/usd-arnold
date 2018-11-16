#include "readVolume.h"

#include <usdKatana/attrMap.h>
#include <usdKatana/readPrim.h>
#include <usdKatana/readXformable.h>

#include <pxr/usd/usdAi/aiVolume.h>

#include <FnAttribute/FnDataBuilder.h>

PXR_NAMESPACE_OPEN_SCOPE


void
readVolume(
        FnKat::GeolibCookInterface& interface,
        FnKat::GroupAttribute opArgs,
        const PxrUsdKatanaUsdInPrivateData& privateData)
{
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

    SdfAssetPath filename;
    volume.GetFilenameAttr().Get(&filename, currentTime);
    argsBuilder.set("filename", FnKat::StringAttribute(filename.GetResolvedPath()));

    float stepSize = 0;
    if (UsdAttribute stepAttr = volume.GetStepSizeAttr()) {
        stepAttr.Get<float>(&stepSize, currentTime);
    }
    argsBuilder.set("step_size", FnKat::FloatAttribute(stepSize));

    // Our KtoA customizations will set up the grid names at translation time.
    argsBuilder.set("grids", FnKat::StringAttribute());
    argsBuilder.set("step_scale", FnKat::FloatAttribute(1.0f));
    argsBuilder.set("volume_padding", FnKat::FloatAttribute(0.0f));
    argsBuilder.set("compress", FnKat::IntAttribute(1));
    argsBuilder.set("velocity_grids", FnKat::StringAttribute());
    argsBuilder.set("velocity_scale", FnKat::FloatAttribute(1.0f));
    argsBuilder.set("velocity_fps", FnKat::FloatAttribute(24));
    argsBuilder.set("velocity_outlier_threshold", FnKat::FloatAttribute(0.001));
    argsBuilder.set("override_motion_range", FnKat::IntAttribute(0));
    argsBuilder.set("makeInteractive", FnKat::StringAttribute("No"));

    // Forward the time slice information to the Arnold op so the motion range 
    // gets set up properly on the volume.
    FnKat::GroupAttribute timeSlice = interface.getOpArg("system.timeSlice");;
    argsBuilder.set("system", FnKat::GroupAttribute("timeSlice", timeSlice, false), false);

    FnKat::GroupAttribute xform;
    if (PxrUsdKatanaReadXformable(volume, privateData, xform)) {
        argsBuilder.set("xform", xform);
    }
    interface.execOp("ArnoldOpenVDBVolume", argsBuilder.build());
}

PXR_NAMESPACE_CLOSE_SCOPE
