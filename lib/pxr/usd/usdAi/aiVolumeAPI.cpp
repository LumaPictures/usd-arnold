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
#include "pxr/usd/usdAi/aiVolumeAPI.h"
#include "pxr/usd/usd/schemaRegistry.h"
#include "pxr/usd/usd/typed.h"
#include "pxr/usd/usd/tokens.h"

#include "pxr/usd/sdf/types.h"
#include "pxr/usd/sdf/assetPath.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the schema with the TfType system.
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<UsdAiVolumeAPI,
        TfType::Bases< UsdAPISchemaBase > >();
    
}

TF_DEFINE_PRIVATE_TOKENS(
    _schemaTokens,
    (AiVolumeAPI)
);

/* virtual */
UsdAiVolumeAPI::~UsdAiVolumeAPI()
{
}

/* static */
UsdAiVolumeAPI
UsdAiVolumeAPI::Get(const UsdStagePtr &stage, const SdfPath &path)
{
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return UsdAiVolumeAPI();
    }
    return UsdAiVolumeAPI(stage->GetPrimAtPath(path));
}


/* virtual */
UsdSchemaType UsdAiVolumeAPI::_GetSchemaType() const {
    return UsdAiVolumeAPI::schemaType;
}

/* static */
UsdAiVolumeAPI
UsdAiVolumeAPI::Apply(const UsdPrim &prim)
{
    return UsdAPISchemaBase::_ApplyAPISchema<UsdAiVolumeAPI>(
            prim, _schemaTokens->AiVolumeAPI);
}

/* static */
const TfType &
UsdAiVolumeAPI::_GetStaticTfType()
{
    static TfType tfType = TfType::Find<UsdAiVolumeAPI>();
    return tfType;
}

/* static */
bool 
UsdAiVolumeAPI::_IsTypedSchema()
{
    static bool isTyped = _GetStaticTfType().IsA<UsdTyped>();
    return isTyped;
}

/* virtual */
const TfType &
UsdAiVolumeAPI::_GetTfType() const
{
    return _GetStaticTfType();
}

UsdAttribute
UsdAiVolumeAPI::GetAiVolumePaddingAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVolume_padding);
}

UsdAttribute
UsdAiVolumeAPI::CreateAiVolumePaddingAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVolume_padding,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiVolumeAPI::GetAiStepSizeAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiStep_size);
}

UsdAttribute
UsdAiVolumeAPI::CreateAiStepSizeAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiStep_size,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiVolumeAPI::GetAiStepScaleAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiStep_scale);
}

UsdAttribute
UsdAiVolumeAPI::CreateAiStepScaleAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiStep_scale,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiVolumeAPI::GetAiCompressAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiCompress);
}

UsdAttribute
UsdAiVolumeAPI::CreateAiCompressAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiCompress,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiVolumeAPI::GetAiVelocityScaleAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVelocity_scale);
}

UsdAttribute
UsdAiVolumeAPI::CreateAiVelocityScaleAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVelocity_scale,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiVolumeAPI::GetAiVelocityFPSAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVelocity_fps);
}

UsdAttribute
UsdAiVolumeAPI::CreateAiVelocityFPSAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVelocity_fps,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiVolumeAPI::GetAiVelocityOutlierThresholdAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVelocity_outlier_threshold);
}

UsdAttribute
UsdAiVolumeAPI::CreateAiVelocityOutlierThresholdAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVelocity_outlier_threshold,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityVarying,
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
UsdAiVolumeAPI::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames = {
        UsdAiTokens->aiVolume_padding,
        UsdAiTokens->aiStep_size,
        UsdAiTokens->aiStep_scale,
        UsdAiTokens->aiCompress,
        UsdAiTokens->aiVelocity_scale,
        UsdAiTokens->aiVelocity_fps,
        UsdAiTokens->aiVelocity_outlier_threshold,
    };
    static TfTokenVector allNames =
        _ConcatenateAttributeNames(
            UsdAPISchemaBase::GetSchemaAttributeNames(true),
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
