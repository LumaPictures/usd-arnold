#include "readProcedural.h"

#include <usdKatana/attrMap.h>

#include <usdKatana/readXformable.h>
#include <usdKatana/usdInPrivateData.h>
#include <usdKatana/utils.h>

#include <pxr/usd/usdAi/aiNodeAPI.h>
#include <pxr/usd/usdAi/aiProcedural.h>
#include <pxr/usd/usdAi/aiVolume.h>

#include <FnAttribute/FnDataBuilder.h>
#include <FnLogging/FnLogging.h>

#include "arnoldHelpers.h"

PXR_NAMESPACE_OPEN_SCOPE

FnLogSetup("readAiProcedural");

void
readAiProcedural(
    const UsdAiProcedural& procedural,
    const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs)
{
    // Read in general attributes for a transformable prim.
    PxrUsdKatanaReadXformable(procedural, data, attrs);

    const double currentTime = data.GetUsdInArgs()->GetCurrentTime();

    // This plugin is registered for both AiProcedural and AiVolume, so check
    // which one we're dealing with, since the handling is slightly different.
    if (procedural.GetPrim().IsA<UsdAiVolume>()) {
        attrs.set("type", FnKat::StringAttribute("volume"));
        attrs.set("geometry.type", FnKat::StringAttribute("volumedso"));
        // TODO: Find a way to check if the "bound" attribute is already set.
        // The current plugin system doesn't give prim reader plugins any way to
        // access the ``GeolibCookInterface`` from the base PxrUsdIn op.
        attrs.set("rendererProcedural.autoBounds", FnAttribute::IntAttribute(1));

        float stepSize = 0;
        if (UsdAttribute stepAttr = UsdAiVolume(procedural).GetStepSizeAttr()) {
            stepAttr.Get<float>(&stepSize, currentTime);
        }
        attrs.set("geometry.step_size", FnKat::FloatAttribute(stepSize));
    } else {
        attrs.set("type", FnKat::StringAttribute("renderer procedural"));
    }

    // Read the DSO value.
    if (UsdAttribute dsoAttr = procedural.GetDsoAttr()) {
        // Not sure if this check is actually necessary, but this attribute
        // doesn't have a default value in the schema, so let's play it safe.
        if (dsoAttr.HasValue()) {
            std::string dso;
            dsoAttr.Get<std::string>(&dso);
            attrs.set("rendererProcedural.procedural",
                      FnKat::StringAttribute(dso));
        }
    }

    // Read all parameters in the "user:" namespace and convert their values to
    // attributes in the "rendererProcedural.args" group attribute.
    // Note that these are only sampled once per frame.
    FnKat::GroupBuilder argsBuilder;

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
