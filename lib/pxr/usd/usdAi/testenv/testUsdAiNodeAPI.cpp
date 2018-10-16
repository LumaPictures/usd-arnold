// Copyright (c) 2018 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
