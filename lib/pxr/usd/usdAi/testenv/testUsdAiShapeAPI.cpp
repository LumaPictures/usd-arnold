#include "pxr/pxr.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usdGeom/mesh.h"

#include "pxr/usd/usdAi/aiShapeAPI.h"

#include <ai.h>

#include <gtest/gtest.h>

PXR_NAMESPACE_USING_DIRECTIVE

constexpr auto framesPerSecond = 24.0;
const SdfPath primPath("/something");

constexpr uint8_t allRays =
    AI_RAY_CAMERA | AI_RAY_SHADOW | AI_RAY_DIFFUSE_TRANSMIT |
    AI_RAY_SPECULAR_TRANSMIT | AI_RAY_VOLUME |
    AI_RAY_DIFFUSE_REFLECT | AI_RAY_SPECULAR_REFLECT | AI_RAY_SUBSURFACE;

constexpr uint8_t autobumpVisibility =
    AI_RAY_CAMERA;

template <typename A>
struct MaskAttributeDefinition {
    UsdAttribute (A::*queryFn)(const VtValue&, bool) const;
    const uint8_t mask;

    inline MaskAttributeDefinition(
        decltype(queryFn) _queryFn,
        uint8_t _mask) : queryFn(_queryFn), mask(_mask) { }
};

template <typename A>
void testMaskAttributes(const A& api,
                        uint8_t (A::*computeFn)() const,
                        const std::vector<MaskAttributeDefinition<A>>& funcs) {
    uint32_t maxValue = 0;
    for (const auto& each: funcs) {
        maxValue |= each.mask;
    }

    // We have to handle cases that are non-continuous
    // but most of the cases they will be, so this is
    // the simplest way of testing all the variations.
    for (auto i = decltype(maxValue){0}; i <= maxValue; ++i) {
        auto exceptedValue = decltype(maxValue){0};
        for (const auto& each: funcs) {
            const auto mask = each.mask & i;
            ((api).*(each.queryFn))(VtValue(), false).Set(mask > 0);
            exceptedValue |= mask;
        }
        EXPECT_EQ(((api).*computeFn)(), exceptedValue);
    }
}

#define SETUP_API() \
auto stage = UsdStage::CreateInMemory("test.usda"); \
stage->SetFramesPerSecond(framesPerSecond); \
auto something = UsdGeomMesh::Define(stage, primPath); \
EXPECT_TRUE(something); \
UsdAiShapeAPI shapeAPI(something.GetPrim()); \
EXPECT_TRUE(shapeAPI);

TEST(USDAiShapeAPI, DefaultVisibility) {
    SETUP_API()
    EXPECT_EQ(shapeAPI.ComputeVisibility(), allRays);
}

TEST(USDAiShapeAPI, DefaultSidedness) {
    SETUP_API()
    EXPECT_EQ(shapeAPI.ComputeSidedness(), allRays);
}

TEST(USDAiShapeAPI, DefaultAutobumpVisibility) {
    SETUP_API()
    EXPECT_EQ(shapeAPI.ComputeSidedness(), allRays);
}

TEST(USDAiShapeAPI, Visibility) {
    SETUP_API()

    const std::vector<MaskAttributeDefinition<UsdAiShapeAPI>> visibilityTest {
        {&UsdAiShapeAPI::CreateAiVisibleToCameraAttr, AI_RAY_CAMERA},
        {&UsdAiShapeAPI::CreateAiVisibleToShadowAttr, AI_RAY_SHADOW},
        {&UsdAiShapeAPI::CreateAiVisibleToDiffuseTransmitAttr, AI_RAY_DIFFUSE_TRANSMIT},
        {&UsdAiShapeAPI::CreateAiVisibleToSpecularTransmitAttr, AI_RAY_SPECULAR_TRANSMIT},
        {&UsdAiShapeAPI::CreateAiVisibleToVolumeAttr, AI_RAY_VOLUME},
        {&UsdAiShapeAPI::CreateAiVisibleToDiffuseReflectAttr, AI_RAY_DIFFUSE_REFLECT},
        {&UsdAiShapeAPI::CreateAiVisibleToSpecularReflectAttr, AI_RAY_SPECULAR_REFLECT},
        {&UsdAiShapeAPI::CreateAiVisibleToSubsurfaceAttr, AI_RAY_SUBSURFACE},
    };

    testMaskAttributes(shapeAPI, &UsdAiShapeAPI::ComputeVisibility, visibilityTest);
}

TEST(USDAiShapeAPI, Sidedness) {
    SETUP_API()

    const std::vector<MaskAttributeDefinition<UsdAiShapeAPI>> sidednessTest {
        {&UsdAiShapeAPI::CreateAiDoubleSidedToCameraAttr, AI_RAY_CAMERA},
        {&UsdAiShapeAPI::CreateAiDoubleSidedToShadowAttr, AI_RAY_SHADOW},
        {&UsdAiShapeAPI::CreateAiDoubleSidedToDiffuseTransmitAttr, AI_RAY_DIFFUSE_TRANSMIT},
        {&UsdAiShapeAPI::CreateAiDoubleSidedToSpecularTransmitAttr, AI_RAY_SPECULAR_TRANSMIT},
        {&UsdAiShapeAPI::CreateAiDoubleSidedToVolumeAttr, AI_RAY_VOLUME},
        {&UsdAiShapeAPI::CreateAiDoubleSidedToDiffuseReflectAttr, AI_RAY_DIFFUSE_REFLECT},
        {&UsdAiShapeAPI::CreateAiDoubleSidedToSpecularReflectAttr, AI_RAY_SPECULAR_REFLECT},
        {&UsdAiShapeAPI::CreateAiDoubleSidedToSubsurfaceAttr, AI_RAY_SUBSURFACE},
    };

    testMaskAttributes(shapeAPI, &UsdAiShapeAPI::ComputeSidedness, sidednessTest);
}

TEST(USDAiShapeAPI, AutobumpVisibility) {
    SETUP_API()

    const std::vector<MaskAttributeDefinition<UsdAiShapeAPI>> autobumpVisibilityTest {
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToCameraAttr, AI_RAY_CAMERA},
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToShadowAttr, AI_RAY_SHADOW},
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToDiffuseTransmitAttr, AI_RAY_DIFFUSE_TRANSMIT},
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToSpecularTransmitAttr, AI_RAY_SPECULAR_TRANSMIT},
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToVolumeAttr, AI_RAY_VOLUME},
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToDiffuseReflectAttr, AI_RAY_DIFFUSE_REFLECT},
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToSpecularReflectAttr, AI_RAY_SPECULAR_REFLECT},
        {&UsdAiShapeAPI::CreateAiAutobumpVisibleToSubsurfaceAttr, AI_RAY_SUBSURFACE},
    };

    testMaskAttributes(shapeAPI, &UsdAiShapeAPI::ComputeAutobumpVisibility, autobumpVisibilityTest);
}
