#include "arnoldHelpers.h"

#include <usdKatana/utils.h>

#include <pxr/usd/usdAi/aiNodeAPI.h>
#include <pxr/usd/usdAi/aiShapeAPI.h>
#include <pxr/usd/usdAi/aiVolumeAPI.h>

#include <pxr/usd/usdGeom/basisCurves.h>
#include <pxr/usd/usdGeom/tokens.h>

PXR_NAMESPACE_OPEN_SCOPE

namespace {
    template <typename T>
    using _attributeDefinition = OptionalAttributeDefinition<T, UsdAiShapeAPI>;
}

std::string
getArnoldAttrTypeHint(const SdfValueTypeName& scalarType)
{
    std::string hint;
    if (scalarType == SdfValueTypeNames->Bool) {
        hint = "boolean";
    }
    else if (scalarType == SdfValueTypeNames->UChar) {
        hint = "byte";
    }
    else if (scalarType == SdfValueTypeNames->UInt ||
             scalarType == SdfValueTypeNames->UInt64) {
        hint = "uint";
    }
    else if (scalarType == SdfValueTypeNames->Matrix4d) {
        hint = "matrix";
    }
    else if (scalarType == SdfValueTypeNames->Float3 ||
             scalarType == SdfValueTypeNames->Double3 ||
             scalarType == SdfValueTypeNames->Half3 ||
             scalarType == SdfValueTypeNames->Vector3f ||
             scalarType == SdfValueTypeNames->Vector3d ||
             scalarType == SdfValueTypeNames->Vector3h ||
             scalarType == SdfValueTypeNames->Normal3f ||
             scalarType == SdfValueTypeNames->Normal3d ||
             scalarType == SdfValueTypeNames->Normal3h ||
             scalarType == SdfValueTypeNames->Point3h ||
             scalarType == SdfValueTypeNames->Point3f ||
             scalarType == SdfValueTypeNames->Point3d) {
        hint = "vector";
    }
    else if (scalarType == SdfValueTypeNames->Float2 ||
             scalarType == SdfValueTypeNames->Double2 ||
             scalarType == SdfValueTypeNames->Half2) {
        hint = "vector2";
    }
    else if (scalarType == SdfValueTypeNames->Color3h ||
             scalarType == SdfValueTypeNames->Color3f ||
             scalarType == SdfValueTypeNames->Color3d) {
        hint = "rgb";
    }
    else if (scalarType == SdfValueTypeNames->Color4h ||
             scalarType == SdfValueTypeNames->Color4f ||
             scalarType == SdfValueTypeNames->Color4d) {
        hint = "rgba";
    }
    return hint;
}

size_t
applyProceduralArgsAttrs(
        const UsdPrim& prim,
        FnKat::GroupBuilder& argsBuilder,
        const double time)
{
    UsdAiNodeAPI nodeAPI(prim);
    size_t count = 0;
    for (const auto& userAttr : nodeAPI.GetUserAttributes()) {
        VtValue vtValue;
        if (!userAttr.Get(&vtValue, time)) {
            continue;
        }

        const std::string attrBaseName = userAttr.GetBaseName().GetString();
        argsBuilder.set(
            attrBaseName,
            PxrUsdKatanaUtils::ConvertVtValueToKatAttr(vtValue, true));
        count++;

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

        static const std::string hintPrefix("arnold_hint__");
        if (!attrHints.empty()) {
            argsBuilder.set(hintPrefix + attrBaseName,
                            FnKat::StringAttribute(attrHints, 2));
        }
    }
    return count;
}


void
getArnoldVDBVolumeOpArgs(
        const UsdPrim& prim,
        FnKat::GroupBuilder& argsBuilder)
{
    static FnKat::GroupAttribute fixedDefaults(
        "grids", FnKat::StringAttribute(),
        "velocity_grids", FnKat::StringAttribute(),
        "override_motion_range", FnKat::IntAttribute(0),
        "makeInteractive", FnKat::StringAttribute("No"),
        true
    );

    static const std::vector<OptionalAttributeDefinition<float, UsdAiVolumeAPI>> floatAttrs = {
        {&UsdAiVolumeAPI::GetAiVolumeStepSizeAttr, "step_size", 0.0f},
        {&UsdAiVolumeAPI::GetAiVolumeStepScaleAttr, "step_scale", 1.0f},
        {&UsdAiVolumeAPI::GetAiVolumePaddingAttr, "volume_padding", 0.0f},
        {&UsdAiVolumeAPI::GetAiVolumeVelocityScaleAttr, "velocity_scale", 1.0f},
        {&UsdAiVolumeAPI::GetAiVolumeVelocityFPSAttr, "velocity_fps", 24.0f},
        {&UsdAiVolumeAPI::GetAiVolumeVelocityOutlierThreshAttr, "velocity_outlier_threshold", 0.001f},
    };

    static const std::vector<OptionalAttributeDefinition<bool, UsdAiVolumeAPI>> boolAttrs = {
        {&UsdAiVolumeAPI::GetAiVolumeCompressAttr, "compress", true},
    };

    argsBuilder.update(fixedDefaults);
    UsdAiVolumeAPI volumeAPI(prim);
    if (volumeAPI) {
        handleAttributes(floatAttrs, volumeAPI, argsBuilder, /*applyDefaults=*/ true);
        handleAttributes(boolAttrs, volumeAPI, argsBuilder, /*applyDefaults=*/ true);
    }
    else {
        // API Schema was not applied to `prim`, but we still want to fill in
        // the defaults. We could skip the above API validity test and just use
        // `handleAttributes` to check all of the attributes directly, but I 
        // would rather standardize on this approach initially, so that authored
        // values are only read if the API was properly applied to the prim.
        for (const auto& entry : floatAttrs) {
            argsBuilder.set(entry.paramName, createAttribute(entry.defaultValue));
        }
        for (const auto& entry : boolAttrs) {
            argsBuilder.set(entry.paramName, createAttribute(entry.defaultValue));
        }
    }
}


FnKat::Attribute
getArnoldStatementsGroup(const UsdPrim& prim) 
{
    UsdAiShapeAPI shapeAPI(prim);
    if (!shapeAPI) {
        // API Schema was not applied to `prim`
        return FnKat::Attribute(); 
    }

    // Sadly std::array needs the size passed as a parameter, so a static const
    // std::vector will do the same in our case.
    static const std::vector<_attributeDefinition<bool>> boolAttrs = {
        {&UsdAiShapeAPI::GetAiVisibleToCameraAttr, "visibility.AI_RAY_CAMERA", true},
        {&UsdAiShapeAPI::GetAiVisibleToShadowAttr, "visibility.AI_RAY_SHADOW", true},
        {&UsdAiShapeAPI::GetAiVisibleToDiffuseTransmitAttr, "visibility.AI_RAY_DIFFUSE_TRANSMIT", true},
        {&UsdAiShapeAPI::GetAiVisibleToSpecularTransmitAttr, "visibility.AI_RAY_SPECULAR_TRANSMIT", true},
        {&UsdAiShapeAPI::GetAiVisibleToVolumeAttr, "visibility.AI_RAY_VOLUME", true},
        {&UsdAiShapeAPI::GetAiVisibleToDiffuseReflectAttr, "visibility.AI_RAY_DIFFUSE_REFLECT", true},
        {&UsdAiShapeAPI::GetAiVisibleToSpecularReflectAttr, "visibility.AI_RAY_SPECULAR_REFLECT", true},
        {&UsdAiShapeAPI::GetAiVisibleToSubsurfaceAttr, "visibility.AI_RAY_SUBSURFACE", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToCameraAttr, "sidedness.AI_RAY_CAMERA", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToShadowAttr, "sidedness.AI_RAY_SHADOW", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToDiffuseTransmitAttr, "sidedness.AI_RAY_DIFFUSE_TRANSMIT", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSpecularTransmitAttr, "sidedness.AI_RAY_SPECULAR_TRANSMIT", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToVolumeAttr, "sidedness.AI_RAY_VOLUME", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToDiffuseReflectAttr, "sidedness.AI_RAY_DIFFUSE_REFLECT", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSpecularReflectAttr, "sidedness.AI_RAY_SPECULAR_REFLECT", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSubsurfaceAttr, "sidedness.AI_RAY_SUBSURFACE", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToCameraAttr, "autobump_visibility.AI_RAY_CAMERA", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToShadowAttr, "autobump_visibility.AI_RAY_SHADOW", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToDiffuseTransmitAttr, "autobump_visibility.AI_RAY_DIFFUSE_TRANSMIT", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToSpecularTransmitAttr, "autobump_visibility.AI_RAY_SPECULAR_TRANSMIT", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToVolumeAttr, "autobump_visibility.AI_RAY_VOLUME", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToDiffuseReflectAttr, "autobump_visibility.AI_RAY_DIFFUSE_REFLECT", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToSpecularReflectAttr, "autobump_visibility.AI_RAY_SPECULAR_REFLECT", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToSubsurfaceAttr, "autobump_visibility.AI_RAY_SUBSURFACE", true},
        // Non visibility attributes where the pattern still applies.
        {&UsdAiShapeAPI::GetAiOpaqueAttr, "opaque", true},
        {&UsdAiShapeAPI::GetAiReceiveShadowsAttr, "receive_shadows", true},
        {&UsdAiShapeAPI::GetAiSelfShadowsAttr, "self_shadows", true},
        // Parameters with false as their default value.
        {&UsdAiShapeAPI::GetAiMatteAttr, "matte", false},
        {&UsdAiShapeAPI::GetAiSmoothingAttr, "smoothing", false},
        {&UsdAiShapeAPI::GetAiSubdivSmoothDerivsAttr, "subdiv_smooth_derivs", false},
        {&UsdAiShapeAPI::GetAiDispAutobumpAttr, "disp_autobump", false}
    };

    static const std::vector<_attributeDefinition<float>> floatAttrs {
        {&UsdAiShapeAPI::GetAiSubdivAdaptiveErrorAttr, "subdiv_adaptive_error", 0.0f},
        {&UsdAiShapeAPI::GetAiDispPaddingAttr, "disp_padding", 0.0f},
        {&UsdAiShapeAPI::GetAiDispHeightAttr, "disp_height", 1.0f},
        {&UsdAiShapeAPI::GetAiDispZeroValueAttr, "zero_value", 0.0f},
        {&UsdAiShapeAPI::GetAiRayBiasAttr, "ray_bias", 0.000001f},
    };

    static const std::vector<_attributeDefinition<unsigned int>> uintAttrs {
        {&UsdAiShapeAPI::GetAiSubdivIterationsAttr, "iterations", 1},
    };

    static const std::vector<_attributeDefinition<TfToken>> stringAttrs {
        {&UsdAiShapeAPI::GetAiSubdivTypeAttr, "subdiv_type", TfToken("none")},
        {&UsdAiShapeAPI::GetAiSubdivAdaptiveMetricAttr, "subdiv_adaptive_metric", TfToken("auto_")},
        {&UsdAiShapeAPI::GetAiSubdivAdaptiveSpaceAttr, "subdiv_adaptive_space", TfToken("raster")},
        {&UsdAiShapeAPI::GetAiSubdivUVSmoothingAttr, "subdiv_uv_smoothing", TfToken("pin_corners")},
        {&UsdAiShapeAPI::GetAiTransformTypeAttr, "transform_type", TfToken("rotate_about_center")},
    };

    FnKat::GroupBuilder builder;

    handleAttributes(boolAttrs, shapeAPI, builder);
    handleAttributes(floatAttrs, shapeAPI, builder);
    handleAttributes(uintAttrs, shapeAPI, builder);
    handleAttributes(stringAttrs, shapeAPI, builder);

    // SubdivAdaptiveMetricAttr will require special handling,
    // if we decide to setup parameters even with the
    // default values. It requires special handling, because one of the values
    // is named auto. The way USD generates tokens, it collides with the c++ auto
    // keyword, so we had to name it auto_.

    // To support width and parameter interpolation for basis curves we have
    // to check if the value of the type and the basis on the BasisCurve to
    // setup arnoldStatements.curve_basis.

    UsdGeomBasisCurves basisCurves(prim);

    if (basisCurves) {
        TfToken type;
        basisCurves.GetTypeAttr().Get<TfToken>(&type);

        std::string curveBasis;
        if (type == UsdGeomTokens->linear) {
            curveBasis = "linear";
        } 
        else {
            TfToken basis;
            basisCurves.GetBasisAttr().Get<TfToken>(&basis);
            if (basis == UsdGeomTokens->bezier) {
                curveBasis = "bezier";
            } 
            else if (basis == UsdGeomTokens->bspline) {
                curveBasis = "b-spline";
            } 
            else if (basis == UsdGeomTokens->catmullRom) {
                curveBasis = "catmull-rom";
            } 
            else {
                TF_WARN("Can't translate basis type %s on %s, falling back to linear.",
                        basis.data(), prim.GetPath().GetString().c_str());
                curveBasis = "linear";
            }
        }
        builder.set("curve_basis", FnKat::StringAttribute(curveBasis));
    }

    return builder.isValid() ? builder.build() : FnKat::Attribute();
}

void 
updateOrCreateAttr(
        FnKat::GeolibCookInterface& interface,
        const std::string& attrName,
        const FnKat::Attribute& attr)
{
    if (!attr.isValid()) {
        return;
    }

    if (attr.getType() == kFnKatAttributeTypeGroup) {
        FnAttribute::GroupAttribute existingAttr = interface.getOutputAttr(attrName);
        if (existingAttr.isValid()) {
            interface.setAttr(
                attrName,
                FnAttribute::GroupBuilder()
                    .update(existingAttr)
                    .deepUpdate(attr)
                    .build());
        }
        else {
            interface.setAttr(attrName, attr);
        }
    }
    else {
        interface.setAttr(attrName, attr);
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
