#include <pxr/pxr.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdShade/tokens.h>
#include <pxr/usd/usdShade/material.h>

#include "pxr/usd/usdAi/aiShaderExport.h"

#include <ai.h>

#include <gtest/gtest.h>

PXR_NAMESPACE_USING_DIRECTIVE

constexpr auto framesPerSecond = 24.0;

#define SETUP_BASE() \
auto stage = UsdStage::CreateInMemory("test.usda"); \
AiShaderExport shaderExport(stage);

struct ArnoldUniverse {
    ArnoldUniverse() { AiBegin(); }
    ~ArnoldUniverse() { AiEnd(); }
};

TEST(UsdAiShaderExport, CleanArnoldName) {
    auto cleanArnoldName = [](const std::string& name) -> std::string {
        auto ret = name;
        AiShaderExport::clean_arnold_name(ret);
        return ret;
    };

    EXPECT_EQ(cleanArnoldName("something"), "something");
    EXPECT_EQ(cleanArnoldName("something@else"), "something_else");
    EXPECT_EQ(cleanArnoldName("something.else"), "something_else");
    EXPECT_EQ(cleanArnoldName("|something|else"), "_something_else");
    EXPECT_EQ(cleanArnoldName("something:else"), "something_else");
    EXPECT_EQ(cleanArnoldName("|something@else:"), "_something_else_");
}

TEST(UsdAiShaderExport, BindMaterial) {
    SETUP_BASE();

    SdfPath somethingPath("/something");
    SdfPath someMaterialPath("/someMaterial");
    SdfPath otherMaterialPath("/otherMaterial");
    auto xform = UsdGeomXform::Define(stage, somethingPath);
    auto material = UsdShadeMaterial::Define(stage, someMaterialPath);
    auto otherMaterial = UsdShadeMaterial::Define(stage, otherMaterialPath);

    shaderExport.bind_material(material.GetPath(), xform.GetPath());

    auto prim = xform.GetPrim();
    auto rel = prim.GetRelationship(UsdShadeTokens->materialBinding);
    SdfPathVector targets;
    rel.GetTargets(&targets);
    EXPECT_EQ(targets.size(), 1);
    EXPECT_EQ(targets[0], someMaterialPath);

    shaderExport.bind_material(otherMaterial.GetPath(), xform.GetPath());
    rel = prim.GetRelationship(UsdShadeTokens->materialBinding);
    rel.GetTargets(&targets);
    EXPECT_EQ(targets.size(), 1);
    EXPECT_EQ(targets[0], otherMaterialPath);
}
