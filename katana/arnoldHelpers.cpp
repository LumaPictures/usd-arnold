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

    // Sadly std::array needs the size passed as a parameter, so a static const
    // std::vector will do the same in our case.
    static const std::vector<std::pair<decltype(&UsdAiShapeAPI::GetAiVisibleToCameraAttr), const char*>> maskAttrs = { {
        {&UsdAiShapeAPI::GetAiVisibleToCameraAttr, "visibility.AI_RAY_CAMERA"},
        {&UsdAiShapeAPI::GetAiVisibleToShadowAttr, "visibility.AI_RAY_SHADOW"},
        {&UsdAiShapeAPI::GetAiVisibleToReflectionAttr, "visibility.AI_RAY_REFLECTED"},
        {&UsdAiShapeAPI::GetAiVisibleToRefractionAttr, "visibility.AI_RAY_REFRACTED"},
        {&UsdAiShapeAPI::GetAiVisibleToSubsurfaceAttr, "visibility.AI_RAY_SUBSURFACE"},
        {&UsdAiShapeAPI::GetAiVisibleToDiffuseAttr, "visibility.AI_RAY_DIFFUSE"},
        {&UsdAiShapeAPI::GetAiVisibleToGlossyAttr, "visibility.AI_RAY_GLOSSY"},
        // Note, the original code for this setup the visibility even when
        // querying sidedness attributes. I asked Nathan if this was intentional.
        // I changed the parameters to sidedness.<ray_type> for now.
        {&UsdAiShapeAPI::GetAiDoubleSidedToCameraAttr, "sidedness.AI_RAY_CAMERA"},
        {&UsdAiShapeAPI::GetAiDoubleSidedToShadowAttr, "sidedness.AI_RAY_SHADOW"},
        {&UsdAiShapeAPI::GetAiDoubleSidedToReflectionAttr, "sidedness.AI_RAY_REFLECTED"},
        {&UsdAiShapeAPI::GetAiDoubleSidedToRefractionAttr, "sidedness.AI_RAY_REFRACTED"},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSubsurfaceAttr, "sidedness.AI_RAY_SUBSURFACE"},
        {&UsdAiShapeAPI::GetAiDoubleSidedToDiffuseAttr, "sidedness.AI_RAY_DIFFUSE"},
        {&UsdAiShapeAPI::GetAiDoubleSidedToGlossyAttr, "sidedness.AI_RAY_GLOSSY"},
        // Non visibility attributes where the pattern still applies.
        {&UsdAiShapeAPI::GetAiOpaqueAttr, "opaque"},
        {&UsdAiShapeAPI::GetAiReceiveShadowsAttr, "receive_shadows"},
        {&UsdAiShapeAPI::GetAiSelfShadowsAttr, "self_shadows"}
    } };

    for (const auto& each : maskAttrs) {
        const auto attr = ((shapeAPI).*(each.first))();
        if (!attr.IsValid()) { continue; }
        bool v = true;
        if (attr.Get(&v) && !v) {
            builder.set(each.second, FnKat::IntAttribute(0));
        }
    }

    // I'm leaving this outside the loop, because I don't want to
    // add just one extra flag to handle things.
    // Matte
    if (UsdAttribute matteAttr = shapeAPI.GetAiMatteAttr()) {
        bool matte = false;
        matteAttr.Get<bool>(&matte);
        if (matte) {
            builder.set("matte", FnKat::IntAttribute(1));
        }
    }

    return builder.build();
}

PXR_NAMESPACE_CLOSE_SCOPE
