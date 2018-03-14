#include "readAOV.h"

/*
    Each group below this location is named usually the same as the
    input AOV in Arnold, like indirect_diffuse etc...

    Each AOV definition has several attributes / groups.
    Attributes
        - channel -> The input AOV name.
        - type -> The typename of the AOV.
        - lightExpressions -> List of strings with the light expressions.
        - name -> The output name of the AOV (?). This seems to be the same
          as the incoming name for now.
        - driver -> The typename of the driver.
        - filter -> The typename of the filter

    Groups
        - driverParameters -> Parameters of the driver. These are the same
          as the parameters on the arnold node.
        - filterParameters -> Parameters of the filter. These are the same
          as the parameters on the arnold node.

*/
PXR_NAMESPACE_OPEN_SCOPE

FnLogSetup("readAOV");


UsdShadeShader
getAOVRelationshipTarget(
    const UsdRelationship& rel)
{
    if (rel) {
        SdfPathVector targets;
        rel.GetTargets(&targets);
        if (!targets.empty()) {
            if (targets.size() > 1) {
                FnLogWarn("Multiple " << rel.GetBaseName() << " relationship "
                          "targets found on AOV at:" << rel.GetPrim().GetPath());
            }
            if (UsdShadeShader result = UsdShadeShader(
                    rel.GetPrim().GetStage()->GetPrimAtPath(targets[0]))) {
                return result;
            }
        }
    }
    return UsdShadeShader();
}


// NOTE: The driver and filter are currently stored as `AiShader` prims, hence
// the name (and behavior) of this function.
TfToken
readAOVChildShaderPrim(
    const UsdShadeShader& shaderSchema,
    FnKat::GroupBuilder& paramsBuilder)
{
    TfToken id;
    shaderSchema.GetIdAttr().Get(&id);

    for (UsdShadeInput shaderInput : shaderSchema.GetInputs()) {
        UsdAttribute shaderAttr = shaderInput.GetAttr();
        VtValue value;
        if (shaderAttr.Get(&value)) {
            paramsBuilder.set(
                shaderInput.GetBaseName(),
                PxrUsdKatanaUtils::ConvertVtValueToKatAttr(value, true));
        }
    }
    return id;
}


// TODO: This op will essentially be obsolete once AOVs are being resolved in
// the main location decorator function.
void
readAiAOV(
    const UsdAiAOV& aov,
    const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs)
{
    FnKat::GroupBuilder aovBuilder;

    std::string name;
    if (aov.GetNameAttr().Get<std::string>(&name)) {
        aovBuilder.set("name", FnKat::StringAttribute(name));
    }

    // No support for reading from another "channel".

    TfToken dataType;
    if (aov.GetDataTypeAttr().Get(&dataType)) {
        aovBuilder.set("datatype", FnKat::StringAttribute(dataType.GetString()));
    }

    if (UsdShadeShader driverSchema =
            getAOVRelationshipTarget(aov.GetDriverRel()))
    {
        FnKat::GroupBuilder driverParamsBuilder;
        const TfToken driverType = readAOVChildShaderPrim(driverSchema,
                                                          driverParamsBuilder);
        aovBuilder.set("driver", FnKat::StringAttribute(driverType));
        aovBuilder.set("driverParameters", driverParamsBuilder.build());
    }

    if (UsdShadeShader filterSchema =
            getAOVRelationshipTarget(aov.GetFilterRel()))
    {
        FnKat::GroupBuilder filterParamsBuilder;
        const TfToken filterType = readAOVChildShaderPrim(filterSchema,
                                                          filterParamsBuilder);
        aovBuilder.set("filter", FnKat::StringAttribute(filterType));
        aovBuilder.set("filterParameters", filterParamsBuilder.build());
    }

    attrs.set("type", FnKat::StringAttribute("aov"));
    attrs.set("aov", aovBuilder.build());
}


PXR_NAMESPACE_CLOSE_SCOPE
