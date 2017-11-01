#include <pxr/pxr.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdShade/tokens.h>
#include <pxr/usd/usdShade/material.h>

#include "pxr/usd/usdAi/aiShaderExport.h"
#include "pxr/usd/usdAi/aiMaterialAPI.h"

#include <pxr/base/gf/vec3f.h>

#include <ai.h>

#include <gtest/gtest.h>

PXR_NAMESPACE_USING_DIRECTIVE

constexpr auto framesPerSecond = 24.0;

#define SETUP_BASE() \
auto stage = UsdStage::CreateInMemory("test.usda"); \
AiShaderExport shaderExport(stage)

struct ArnoldUniverse {
    ArnoldUniverse() { AiBegin(); }
    ~ArnoldUniverse() { AiEnd(); }
};

#define SETUP_UNIVERSE() \
ArnoldUniverse arnoldUniverse

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

template <typename T>
bool equalParam(const UsdPrim& prim, const std::string paramName, const T& paramValue, UsdTimeCode time = UsdTimeCode::Default()) {
    T ret;
    const auto attr = prim.GetAttribute(TfToken(paramName));
    attr.Get(&ret, time);
    return ret == paramValue;
}

TEST(UsdAiShaderExport, ExportArnoldNode) {
    SETUP_UNIVERSE();
    SETUP_BASE();

    auto* standard1 = AiNode(AtString("standard"));
    AiNodeSetStr(standard1, AtString("name"), AtString("standard1"));
    AiNodeSetFlt(standard1, AtString("Kt"), 0.8f);
    AiNodeSetFlt(standard1, AtString("Kr"), 0.7f);
    AiNodeSetRGB(standard1, AtString("Ks_color"), 0.5f, 0.12f, 0.4f);

    auto standard1Path = shaderExport.export_arnold_node(standard1, SdfPath("/"));
    EXPECT_EQ(standard1Path, SdfPath("/standard1"));
    auto prim = stage->GetPrimAtPath(standard1Path);
    EXPECT_TRUE(prim);
    EXPECT_TRUE(equalParam(prim, "info:id", TfToken("standard")));
    EXPECT_TRUE(equalParam(prim, "inputs:Kt", 0.8f));
    EXPECT_TRUE(equalParam(prim, "inputs:Kr", 0.7f));
    EXPECT_TRUE(equalParam(prim, "inputs:Ks_color", GfVec3f(0.5f, 0.12f, 0.4f)));

    auto* standard2 = AiNode(AtString("standard"));
    AiNodeSetStr(standard2, AtString("name"), AtString("standard2"));
    AiNodeSetFlt(standard2, AtString("Kt"), 0.5f);
    AiNodeSetFlt(standard2, AtString("Kr"), 0.42f);

    std::set<std::string> exportableParams = {"Kt"};
    auto standard2Path = shaderExport.export_arnold_node(standard2, SdfPath("/"), &exportableParams);
    EXPECT_EQ(standard2Path, SdfPath("/standard2"));
    prim = stage->GetPrimAtPath(standard2Path);
    EXPECT_TRUE(prim);
    EXPECT_TRUE(equalParam(prim, "info:id", TfToken("standard")));
    EXPECT_TRUE(equalParam(prim, "inputs:Kt", 0.5f));
    EXPECT_FALSE(prim.GetAttribute(TfToken("inputs:Kr")));
}

bool checkRelationship(const UsdRelationship& rel, const SdfPath& path) {
    SdfPathVector paths;
    rel.GetTargets(&paths);
    if (paths.size() != 1) { return false; }
    return paths[0] == path;
}

TEST(UsdAiShaderExport, ExportMaterial) {
    SETUP_UNIVERSE();
    SETUP_BASE();

    auto* hair = AiNode(AtString("hair"));
    AiNodeSetStr(hair, AtString("name"), AtString("hair"));
    auto* noise = AiNode(AtString("noise"));
    AiNodeSetStr(noise, AtString("name"), AtString("noise"));

    auto material = shaderExport.export_material("myMaterial", hair, noise);
    auto materialPrim = stage->GetPrimAtPath(material);
    auto hairPrim = stage->GetPrimAtPath(SdfPath("/Looks/hair"));
    auto noisePrim = stage->GetPrimAtPath(SdfPath("/Looks/noise"));

    EXPECT_TRUE(materialPrim);
    EXPECT_TRUE(hairPrim);
    EXPECT_TRUE(noisePrim);

    EXPECT_TRUE(materialPrim.IsA<UsdShadeMaterial>());
    EXPECT_TRUE(hairPrim.IsA<UsdAiShader>());
    EXPECT_TRUE(noisePrim.IsA<UsdAiShader>());

    EXPECT_TRUE(equalParam(hairPrim, "info:id", TfToken("hair")));
    EXPECT_TRUE(equalParam(noisePrim, "info:id", TfToken("noise")));

    EXPECT_EQ(materialPrim.GetPath(), SdfPath("/Looks/myMaterial"));
    UsdAiMaterialAPI materialAPI(materialPrim);
    EXPECT_TRUE(materialAPI);
    EXPECT_TRUE(checkRelationship(materialAPI.GetSurfaceRel(), SdfPath("/Looks/hair")));
    EXPECT_TRUE(checkRelationship(materialAPI.GetDisplacementRel(), SdfPath("/Looks/noise")));
}

TEST(UsdAiShaderExport, ParentScope) {
    SETUP_UNIVERSE();
    auto stage = UsdStage::CreateInMemory("test.usda");
    AiShaderExport shaderExport(stage, SdfPath("/something/else"));

    auto* hair = AiNode(AtString("hair"));
    AiNodeSetStr(hair, AtString("name"), AtString("hair"));
    auto* noise = AiNode(AtString("noise"));
    AiNodeSetStr(noise, AtString("name"), AtString("noise"));

    auto materialPath = shaderExport.export_material("myMaterial", hair, noise);
    auto materialPrim = stage->GetPrimAtPath(materialPath);
    auto hairPrim = stage->GetPrimAtPath(SdfPath("/something/else/hair"));
    auto noisePrim = stage->GetPrimAtPath(SdfPath("/something/else/noise"));

    EXPECT_TRUE(materialPrim);
    EXPECT_TRUE(hairPrim);
    EXPECT_TRUE(noisePrim);

    EXPECT_EQ(materialPath, SdfPath("/something/else/myMaterial"));
    UsdAiMaterialAPI materialAPI(materialPrim);
    EXPECT_TRUE(checkRelationship(materialAPI.GetSurfaceRel(), SdfPath("/something/else/hair")));
    EXPECT_TRUE(checkRelationship(materialAPI.GetDisplacementRel(), SdfPath("/something/else/noise")));
}
