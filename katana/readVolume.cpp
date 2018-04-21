#include "readVolume.h"

#include <usdKatana/attrMap.h>

#include <usdKatana/readXformable.h>
#include <usdKatana/usdInPrivateData.h>

#include <pxr/usd/usdAi/aiVolume.h>

#include <FnAttribute/FnDataBuilder.h>
#include <FnLogging/FnLogging.h>

#include "arnoldHelpers.h"

PXR_NAMESPACE_OPEN_SCOPE


void
readVolume(
        const UsdAiVolume& volume,
        const PxrUsdKatanaUsdInPrivateData& data,
        PxrUsdKatanaAttrMap& attrs)
{
    PxrUsdKatanaReadXformable(volume, data, attrs);
    const auto currentTime = data.GetUsdInArgs()->GetCurrentTime();

    attrs.set("type", FnKat::StringAttribute("volume"));
    float stepSize = 0;
    if (UsdAttribute stepAttr = UsdAiVolume(volume).GetStepSizeAttr()) {
        stepAttr.Get<float>(&stepSize, currentTime);
    }
    attrs.set("arnoldStatements.step_size", FnKat::FloatAttribute(stepSize));
    attrs.set("geometry.type", FnKat::StringAttribute("volumeplugin"));
    attrs.set("rendererProcedural.node", FnKat::StringAttribute("volume"));
    attrs.set("rendererProcedural.autoBounds", FnKat::IntAttribute(1));

    FnKat::GroupBuilder argsBuilder;

    // our KtoA version will handle this
    argsBuilder.set("grids", FnKat::StringAttribute(""));
    argsBuilder.set("__skipBuiltins", FnKat::IntAttribute(1));
    argsBuilder.set("__outputStyle", FnKat::StringAttribute("typedArguments"));

    SdfAssetPath filename;
    volume.GetFilenameAttr().Get(&filename, currentTime);
    argsBuilder.set("filename", FnKat::StringAttribute(filename.GetResolvedPath()));

    applyProceduralArgsAttrs(volume.GetPrim(), argsBuilder, currentTime);
    attrs.set("rendererProcedural.args", argsBuilder.build());
}

PXR_NAMESPACE_CLOSE_SCOPE
