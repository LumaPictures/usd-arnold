// Copyright 2019 Luma Pictures
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "pxr/pxr.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usdGeom/mesh.h"

#include "pxr/usd/usdAi/aiNodeAPI.h"
#include "pxr/usd/usdAi/tokens.h"

#include <ai.h>

#include <gtest/gtest.h>

PXR_NAMESPACE_USING_DIRECTIVE

constexpr auto framesPerSecond = 24.0;
const SdfPath primPath("/something");

static const TfToken varName1("myCustomVariable");
static const TfToken varName2("thisIsAnotherVariable");
static const TfToken varName3("isThisAVariable");

static const TfToken varNamePrefixed1(
    UsdAiTokens->userPrefix.GetString() + varName1.GetString());
static const TfToken varNamePrefixed2(
    UsdAiTokens->userPrefix.GetString() + varName2.GetString());
static const TfToken varNamePrefixed3(
    UsdAiTokens->userPrefix.GetString() + varName3.GetString());

static const auto varType1 = SdfValueTypeNames -> Float;
static const auto varType2 = SdfValueTypeNames -> Bool;
static const auto varType3 = SdfValueTypeNames -> Color3d;

#define SETUP_API()                                          \
    auto stage = UsdStage::CreateInMemory("test.usda");      \
    stage->SetFramesPerSecond(framesPerSecond);              \
    auto something = UsdGeomMesh::Define(stage, primPath);   \
    EXPECT_TRUE(something);                                  \
    auto nodeAPI = UsdAiNodeAPI::Apply(something.GetPrim()); \
    EXPECT_TRUE(nodeAPI);

#define SETUP_PARAMETERS()                           \
    nodeAPI.CreateUserAttribute(varName1, varType1); \
    nodeAPI.CreateUserAttribute(varName2, varType2); \
    nodeAPI.CreateUserAttribute(varName3, varType3);

#define VALIDATE_PARAMETERS()                \
    EXPECT_TRUE(var1);                       \
    EXPECT_TRUE(var2);                       \
    EXPECT_TRUE(var3);                       \
    EXPECT_EQ(var1.GetTypeName(), varType1); \
    EXPECT_EQ(var2.GetTypeName(), varType2); \
    EXPECT_EQ(var3.GetTypeName(), varType3); \
    EXPECT_EQ(var1.GetBaseName(), varName1); \
    EXPECT_EQ(var2.GetBaseName(), varName2); \
    EXPECT_EQ(var3.GetBaseName(), varName3);

TEST(USDAiNodeAPI, Creation) { SETUP_API(); }

TEST(USDAiNodeAPI, AttributeCreation) {
    SETUP_API();

    SETUP_PARAMETERS();

    auto prim = something.GetPrim();

    const auto var1 = prim.GetAttribute(varNamePrefixed1);
    const auto var2 = prim.GetAttribute(varNamePrefixed2);
    const auto var3 = prim.GetAttribute(varNamePrefixed3);

    VALIDATE_PARAMETERS();
}

TEST(USDAiNodeAPI, AttributeAccess) {
    SETUP_API();

    SETUP_PARAMETERS();

    const auto var1 = nodeAPI.GetUserAttribute(varName1);
    const auto var2 = nodeAPI.GetUserAttribute(varName2);
    const auto var3 = nodeAPI.GetUserAttribute(varName3);

    VALIDATE_PARAMETERS();
}

TEST(USDAiNodeAPI, ListAttributes) {
    SETUP_API();

    SETUP_PARAMETERS();

    const auto vars = nodeAPI.GetUserAttributes();
    // This returns the variables in order,
    // so we have to order them around.
    const auto var1 = vars[1];
    const auto var2 = vars[2];
    const auto var3 = vars[0];

    VALIDATE_PARAMETERS();
}
