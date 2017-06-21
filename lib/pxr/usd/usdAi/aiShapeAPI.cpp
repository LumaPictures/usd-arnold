//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxr/usd/usdAi/aiShapeAPI.h"
#include "pxr/usd/usd/schemaRegistry.h"
#include "pxr/usd/usd/typed.h"

#include "pxr/usd/sdf/types.h"
#include "pxr/usd/sdf/assetPath.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the schema with the TfType system.
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<UsdAiShapeAPI,
        TfType::Bases< UsdSchemaBase > >();
    
}

/* virtual */
UsdAiShapeAPI::~UsdAiShapeAPI()
{
}

/* static */
UsdAiShapeAPI
UsdAiShapeAPI::Get(const UsdStagePtr &stage, const SdfPath &path)
{
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return UsdAiShapeAPI();
    }
    return UsdAiShapeAPI(stage->GetPrimAtPath(path));
}


/* static */
const TfType &
UsdAiShapeAPI::_GetStaticTfType()
{
    static TfType tfType = TfType::Find<UsdAiShapeAPI>();
    return tfType;
}

/* static */
bool 
UsdAiShapeAPI::_IsTypedSchema()
{
    static bool isTyped = _GetStaticTfType().IsA<UsdTyped>();
    return isTyped;
}

/* virtual */
const TfType &
UsdAiShapeAPI::_GetTfType() const
{
    return _GetStaticTfType();
}

UsdAttribute
UsdAiShapeAPI::GetOpaqueAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->opaque);
}

UsdAttribute
UsdAiShapeAPI::CreateOpaqueAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->opaque,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetMatteAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->matte);
}

UsdAttribute
UsdAiShapeAPI::CreateMatteAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->matte,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetReceiveShadowsAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->receive_shadows);
}

UsdAttribute
UsdAiShapeAPI::CreateReceiveShadowsAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->receive_shadows,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetSelfShadowsAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->self_shadows);
}

UsdAttribute
UsdAiShapeAPI::CreateSelfShadowsAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->self_shadows,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetVisibilityAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->visibility);
}

UsdAttribute
UsdAiShapeAPI::CreateVisibilityAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->visibility,
                       SdfValueTypeNames->UChar,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetSidednessAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->sidedness);
}

UsdAttribute
UsdAiShapeAPI::CreateSidednessAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->sidedness,
                       SdfValueTypeNames->UChar,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

namespace {
static inline TfTokenVector
_ConcatenateAttributeNames(const TfTokenVector& left,const TfTokenVector& right)
{
    TfTokenVector result;
    result.reserve(left.size() + right.size());
    result.insert(result.end(), left.begin(), left.end());
    result.insert(result.end(), right.begin(), right.end());
    return result;
}
}

/*static*/
const TfTokenVector&
UsdAiShapeAPI::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames = {
        UsdAiTokens->opaque,
        UsdAiTokens->matte,
        UsdAiTokens->receive_shadows,
        UsdAiTokens->self_shadows,
        UsdAiTokens->visibility,
        UsdAiTokens->sidedness,
    };
    static TfTokenVector allNames =
        _ConcatenateAttributeNames(
            UsdSchemaBase::GetSchemaAttributeNames(true),
            localNames);

    if (includeInherited)
        return allNames;
    else
        return localNames;
}

PXR_NAMESPACE_CLOSE_SCOPE

// ===================================================================== //
// Feel free to add custom code below this line. It will be preserved by
// the code generator.
//
// Just remember to wrap code in the appropriate delimiters:
// 'PXR_NAMESPACE_OPEN_SCOPE', 'PXR_NAMESPACE_CLOSE_SCOPE'.
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--
