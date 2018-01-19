#include "arnoldHelpers.h"

#include <pxr/usd/usdAi/aiShapeAPI.h>

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
             scalarType == SdfValueTypeNames->Normal3h) {
        hint = "vector";
    }
    else if (scalarType == SdfValueTypeNames->Float2 ||
             scalarType == SdfValueTypeNames->Double2 ||
             scalarType == SdfValueTypeNames->Half2) {
        hint = "point2";
    }
    else if (scalarType == SdfValueTypeNames->Point3h ||
             scalarType == SdfValueTypeNames->Point3f ||
             scalarType == SdfValueTypeNames->Point3d) {
        hint = "point";
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


FnKat::Attribute
getArnoldStatementsGroup(const UsdPrim& prim) {
    FnKat::GroupBuilder builder;

    UsdAiShapeAPI shapeAPI(prim);
    if (!shapeAPI) { return FnKat::Attribute(); }

    // Sadly std::array needs the size passed as a parameter, so a static const
    // std::vector will do the same in our case.
    static const std::vector<_attributeDefinition<bool>> boolAttrs = {
#ifdef ARNOLD5
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
#else
        {&UsdAiShapeAPI::GetAiVisibleToCameraAttr, "visibility.AI_RAY_CAMERA", true},
        {&UsdAiShapeAPI::GetAiVisibleToShadowAttr, "visibility.AI_RAY_SHADOW", true},
        {&UsdAiShapeAPI::GetAiVisibleToReflectionAttr, "visibility.AI_RAY_REFLECTED", true},
        {&UsdAiShapeAPI::GetAiVisibleToRefractionAttr, "visibility.AI_RAY_REFRACTED", true},
        {&UsdAiShapeAPI::GetAiVisibleToSubsurfaceAttr, "visibility.AI_RAY_SUBSURFACE", true},
        {&UsdAiShapeAPI::GetAiVisibleToDiffuseAttr, "visibility.AI_RAY_DIFFUSE", true},
        {&UsdAiShapeAPI::GetAiVisibleToGlossyAttr, "visibility.AI_RAY_GLOSSY", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToCameraAttr, "sidedness.AI_RAY_CAMERA", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToShadowAttr, "sidedness.AI_RAY_SHADOW", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToReflectionAttr, "sidedness.AI_RAY_REFLECTED", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToRefractionAttr, "sidedness.AI_RAY_REFRACTED", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSubsurfaceAttr, "sidedness.AI_RAY_SUBSURFACE", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToDiffuseAttr, "sidedness.AI_RAY_DIFFUSE", true},
        {&UsdAiShapeAPI::GetAiDoubleSidedToGlossyAttr, "sidedness.AI_RAY_GLOSSY", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToCameraAttr, "autobump_visibility.AI_RAY_CAMERA", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToShadowAttr, "autobump_visibility.AI_RAY_SHADOW", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToReflectionAttr, "autobump_visibility.AI_RAY_REFLECTED", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToRefractionAttr, "autobump_visibility.AI_RAY_REFRACTED", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToSubsurfaceAttr, "autobump_visibility.AI_RAY_SUBSURFACE", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToDiffuseAttr, "autobump_visibility.AI_RAY_DIFFUSE", true},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToGlossyAttr, "autobump_visibility.AI_RAY_GLOSSY", true},
#endif
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
#ifdef ARNOLD5
        {&UsdAiShapeAPI::GetAiTransformTypeAttr, "transform_type", TfToken("rotate_about_center")},
#endif
    };

    auto needToBuild = handleAttributes(boolAttrs, shapeAPI, builder);
    needToBuild |= handleAttributes(floatAttrs, shapeAPI, builder);
    needToBuild |= handleAttributes(uintAttrs, shapeAPI, builder);
    needToBuild |= handleAttributes(stringAttrs, shapeAPI, builder);

    // SubdivAdaptiveMetricAttr will require special handling,
    // if we decide to setup parameters even with the
    // default values. It requires special handling, because one of the values
    // is named auto. The way USD generates tokens, it collides with the c++ auto
    // keyword, so we had to name it auto_.

    return needToBuild ? builder.build() : FnKat::Attribute();
}

void updateOrCreateAttr(
    FnKat::GeolibCookInterface& interface,
    const std::string& attrName,
    const FnKat::Attribute& attr) {
    if (!attr.isValid()) { return;  }

    if (attr.getType() == kFnKatAttributeTypeGroup) {
        FnAttribute::GroupAttribute existingAttr = interface.getOutputAttr(attrName);
        if (existingAttr.isValid()) {
            interface.setAttr(attrName
                , FnAttribute::GroupBuilder()
                                  .update(existingAttr)
                                  .deepUpdate(attr)
                                  .build());
        } else {
            interface.setAttr(attrName, attr);
        }
    } else {
        interface.setAttr(attrName, attr);
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
