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


// NOTE: The driver and filter are currently stored as `AiShader` prims, hence
// the name (and behavior) of this function.
static std::string
_readChildShaderPrim(
    const UsdShadeShader& shaderSchema,
    FnKat::GroupBuilder& paramsBuilder)
{
    TfToken id;
    shaderSchema.GetIdAttr().Get(&id);

    std::vector<UsdShadeInput> shaderInputs = shaderSchema.GetInputs();
    TF_FOR_ALL(shaderInputIter, shaderInputs) {
        UsdShadeInput shaderInput = *shaderInputIter;
        UsdAttribute shaderAttr = shaderInput.GetAttr();
        VtValue value;
        if (shaderAttr.Get(&value)) {
            paramsBuilder.set(
                shaderInput.GetBaseName(),
                PxrUsdKatanaUtils::ConvertVtValueToKatAttr(value, true));
        }
    }

    return id.GetString();
}


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

    if (UsdRelationship driverRel = aov.GetDriverRel()) {
        SdfPathVector driverTargets;
        driverRel.GetTargets(&driverTargets);
        if (driverTargets.size() > 0) {
            if (driverTargets.size() > 1) {
                FnLogWarn("Multiple driver relationship targets found on AOV "
                          "at:" << data.GetUsdPrim().GetPath());
            }
            if (UsdShadeShader driverSchema = UsdShadeShader(
                    data.GetUsdInArgs()->GetStage()->GetPrimAtPath(driverTargets[0]))) {
                FnKat::GroupBuilder driverParamsBuilder;
                const std::string driverType = _readChildShaderPrim(
                    driverSchema, driverParamsBuilder);
                aovBuilder.set("driver", FnKat::StringAttribute(driverType));
                aovBuilder.set("driverParameters", driverParamsBuilder.build());
            }
        }
    }

    if (UsdRelationship filterRel = aov.GetFilterRel()) {
        SdfPathVector filterTargets;
        filterRel.GetTargets(&filterTargets);
        if (filterTargets.size() > 0) {
            if (filterTargets.size() > 1) {
                FnLogWarn("Multiple filter relationship targets found on AOV "
                          "at:" << data.GetUsdPrim().GetPath());
            }
            if (UsdShadeShader filterSchema = UsdShadeShader(
                    data.GetUsdInArgs()->GetStage()->GetPrimAtPath(filterTargets[0]))) {
                FnKat::GroupBuilder filterParamsBuilder;
                const std::string filterType =_readChildShaderPrim(
                    filterSchema, filterParamsBuilder);
                aovBuilder.set("filter", FnKat::StringAttribute(filterType));
                aovBuilder.set("filterParameters", filterParamsBuilder.build());
            }
        }
    }

    attrs.set("type", FnKat::StringAttribute("aov"));
    attrs.set("aov", aovBuilder.build());
}


PXR_NAMESPACE_CLOSE_SCOPE
