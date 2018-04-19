#include "readProcedural.h"

#include <usdKatana/attrMap.h>

#include <usdKatana/readXformable.h>
#include <usdKatana/usdInPrivateData.h>
#include <usdKatana/utils.h>

#include <pxr/usd/usdAi/aiNodeAPI.h>
#include <pxr/usd/usdAi/aiProcedural.h>
#include <pxr/usd/usdAi/aiVolume.h>

#include <FnAttribute/FnDataBuilder.h>

#include "arnoldHelpers.h"

PXR_NAMESPACE_OPEN_SCOPE


void
readAiProcedural(
    const UsdAiProcedural& procedural,
    const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs)
{
    // Read in general attributes for a transformable prim.
    PxrUsdKatanaReadXformable(procedural, data, attrs);

    const double currentTime = data.GetUsdInArgs()->GetCurrentTime();

    attrs.set("type", FnKat::StringAttribute("renderer procedural"));

    if (auto idAttr = procedural.GetIdAttr()) {
        if (idAttr.HasValue()) {
            TfToken id;
            idAttr.Get(&id);
            attrs.set("rendererProcedural.procedural",
                      FnKat::StringAttribute(id.GetString()));
        }
    }

    // Read all parameters in the "user:" namespace and convert their values to
    // attributes in the "rendererProcedural.args" group attribute.
    // Note that these are only sampled once per frame.
    FnKat::GroupBuilder argsBuilder;
    argsBuilder.set("__outputStyle", FnKat::StringAttribute("typedArguments"));

    UsdAiNodeAPI nodeAPI = UsdAiNodeAPI(procedural);
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
            attrHints.push_back("array");
            attrHints.push_back("true");
        }

        std::string typeHint = getArnoldAttrTypeHint(
            userAttr.GetTypeName().GetScalarType());
        if (!typeHint.empty()) {
            attrHints.push_back("type");
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
