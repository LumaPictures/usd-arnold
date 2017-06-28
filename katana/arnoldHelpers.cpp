#include "arnoldHelpers.h"

#include <pxr/usd/usdAi/aiShapeAPI.h>
#include <pxr/usd/usdAi/rayTypes4.h>

using namespace arnold4;

PXR_NAMESPACE_OPEN_SCOPE

std::string
GetArnoldAttrTypeHint(const SdfValueTypeName& scalarType)
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
GetArnoldStatementsGroup(const UsdPrim& prim) {
    FnKat::GroupBuilder builder;

    UsdAiShapeAPI shapeAPI(prim);

    // Visibility, by ray type
    if (UsdAttribute visibilityAttr = shapeAPI.GetVisibilityAttr()) {
        uint8_t visibilityMask = 255;
        visibilityAttr.Get<uint8_t>(&visibilityMask);

        if (!(visibilityMask & AI_RAY_CAMERA)) {
            builder.set("visibility.AI_RAY_CAMERA", FnKat::IntAttribute(0));
        }
        if (!(visibilityMask & AI_RAY_SHADOW)) {
            builder.set("visibility.AI_RAY_SHADOW", FnKat::IntAttribute(0));
        }
        if (!(visibilityMask & AI_RAY_REFLECTED)) {
            builder.set("visibility.AI_RAY_REFLECTED", FnKat::IntAttribute(0));
        }
        if (!(visibilityMask & AI_RAY_REFRACTED)) {
            builder.set("visibility.AI_RAY_REFRACTED", FnKat::IntAttribute(0));
        }
        if (!(visibilityMask & AI_RAY_SUBSURFACE)) {
            builder.set("visibility.AI_RAY_SUBSURFACE", FnKat::IntAttribute(0));
        }
        if (!(visibilityMask & AI_RAY_DIFFUSE)) {
            builder.set("visibility.AI_RAY_DIFFUSE", FnKat::IntAttribute(0));
        }
        if (!(visibilityMask & AI_RAY_GLOSSY)) {
            builder.set("visibility.AI_RAY_GLOSSY", FnKat::IntAttribute(0));
        }
    }

    // Sidedness, by ray type
    if (UsdAttribute sidednessAttr = shapeAPI.GetSidednessAttr()) {
        uint8_t sidednessMask = 255;
        sidednessAttr.Get<uint8_t>(&sidednessMask);

        if (!(sidednessMask & AI_RAY_CAMERA)) {
            builder.set("visibility.AI_RAY_CAMERA", FnKat::IntAttribute(0));
        }
        if (!(sidednessMask & AI_RAY_SHADOW)) {
            builder.set("visibility.AI_RAY_SHADOW", FnKat::IntAttribute(0));
        }
        if (!(sidednessMask & AI_RAY_REFLECTED)) {
            builder.set("visibility.AI_RAY_REFLECTED", FnKat::IntAttribute(0));
        }
        if (!(sidednessMask & AI_RAY_REFRACTED)) {
            builder.set("visibility.AI_RAY_REFRACTED", FnKat::IntAttribute(0));
        }
        if (!(sidednessMask & AI_RAY_SUBSURFACE)) {
            builder.set("visibility.AI_RAY_SUBSURFACE", FnKat::IntAttribute(0));
        }
        if (!(sidednessMask & AI_RAY_DIFFUSE)) {
            builder.set("visibility.AI_RAY_DIFFUSE", FnKat::IntAttribute(0));
        }
        if (!(sidednessMask & AI_RAY_GLOSSY)) {
            builder.set("visibility.AI_RAY_GLOSSY", FnKat::IntAttribute(0));
        }
    }

    // TODO(?)
    // inline void
    // _ApplyBooleanAttr(const char* name, const UsdAttribute& attr,
    //                   const bool defaultState, FnKat::GroupBuilder& builder) {
    //     bool value = defaultState;
    //     attr.Get<bool>(&value);
    //     if (value != defaultState) {
    //         builder.set(name, FnKat::IntAttribute((int)value));
    //     }
    // }

    // Opaque
    if (UsdAttribute opaqueAttr = shapeAPI.GetOpaqueAttr()) {
        bool opaque = true;
        opaqueAttr.Get<bool>(&opaque);
        if (!opaque) {
            builder.set("opaque", FnKat::IntAttribute(0));
        }
    }

    // Matte
    if (UsdAttribute matteAttr = shapeAPI.GetMatteAttr()) {
        bool matte = false;
        matteAttr.Get<bool>(&matte);
        if (matte) {
            builder.set("matte", FnKat::IntAttribute(1));
        }
    }

    // Receive Shadows
    if (UsdAttribute receiveShadowsAttr = shapeAPI.GetReceiveShadowsAttr()) {
        bool receiveShadows = true;
        receiveShadowsAttr.Get<bool>(&receiveShadows);
        if (!receiveShadows) {
            builder.set("receive_shadows", FnKat::IntAttribute(0));
        }
    }

    // Self Shadows
    if (UsdAttribute selfShadowsAttr = shapeAPI.GetSelfShadowsAttr()) {
        bool selfShadows = true;
        selfShadowsAttr.Get<bool>(&selfShadows);
        if (!selfShadows) {
            builder.set("self_shadows", FnKat::IntAttribute(0));
        }
    }

    return builder.build();
}

PXR_NAMESPACE_CLOSE_SCOPE
