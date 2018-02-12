#include "readVolume.h"

#include <usdKatana/attrMap.h>

#include <usdKatana/readXformable.h>
#include <usdKatana/usdInPrivateData.h>
#include <usdKatana/utils.h>

#include <pxr/usd/usdAi/aiNodeAPI.h>
#include <pxr/usd/usdAi/aiVolume.h>

#include <FnAttribute/FnDataBuilder.h>
#include <FnLogging/FnLogging.h>

#include "arnoldHelpers.h"

PXR_NAMESPACE_OPEN_SCOPE

void
readVolume(
    const UsdAiVolume& volume,
    const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs) {
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
    SdfAssetPath filename;
    volume.GetFilenameAttr().Get(&filename, currentTime);
    argsBuilder.set("filename", FnKat::StringAttribute(filename.GetResolvedPath()));
    UsdAiNodeAPI nodeAPI = UsdAiNodeAPI(volume);
    std::vector<UsdAttribute> userAttrs = nodeAPI.GetUserAttributes();
    TF_FOR_ALL(attrIter, userAttrs) {
        UsdAttribute userAttr = *attrIter;
        VtValue vtValue;
        if (!userAttr.Get(&vtValue, currentTime)) {
            continue;
        }

        const std::string attrBaseName = userAttr.GetBaseName().GetString();
        argsBuilder.set(
            attrBaseName,
            PxrUsdKatanaUtils::ConvertVtValueToKatAttr(vtValue, true));

        // Create KtoA hint attribute if necessary.
        std::vector<std::string> attrHints;
        if (userAttr.GetTypeName().IsArray()) {
            attrHints.emplace_back("array");
            attrHints.emplace_back("true");
        }

        std::string typeHint = getArnoldAttrTypeHint(
            userAttr.GetTypeName().GetScalarType());
        if (!typeHint.empty()) {
            attrHints.emplace_back("type");
            attrHints.push_back(typeHint);
        }
        // TODO(?): `key_array` and `clone` hints

        if (!attrHints.empty()) {
            argsBuilder.set(std::string("arnold_hint__") + attrBaseName,
                            FnKat::StringAttribute(attrHints, 2));
        }
    }

    attrs.set("rendererProcedural.args", argsBuilder.build());
}

PXR_NAMESPACE_CLOSE_SCOPE
