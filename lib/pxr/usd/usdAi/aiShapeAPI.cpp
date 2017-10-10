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
UsdAiShapeAPI::GetAiOpaqueAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiOpaque);
}

UsdAttribute
UsdAiShapeAPI::CreateAiOpaqueAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiOpaque,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiMatteAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiMatte);
}

UsdAttribute
UsdAiShapeAPI::CreateAiMatteAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiMatte,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiRayBiasAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiRay_bias);
}

UsdAttribute
UsdAiShapeAPI::CreateAiRayBiasAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiRay_bias,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiReceiveShadowsAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiReceive_shadows);
}

UsdAttribute
UsdAiShapeAPI::CreateAiReceiveShadowsAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiReceive_shadows,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSelfShadowsAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSelf_shadows);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSelfShadowsAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSelf_shadows,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiTransformTypeAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiTransform_type);
}

UsdAttribute
UsdAiShapeAPI::CreateAiTransformTypeAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiTransform_type,
                       SdfValueTypeNames->Token,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToCameraAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilityCamera);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToCameraAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilityCamera,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToShadowAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilityShadow);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToShadowAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilityShadow,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToDiffuseTransmitAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilityDiffuse_transmit);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToDiffuseTransmitAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilityDiffuse_transmit,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToSpecularTransmitAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilitySpecular_transmit);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToSpecularTransmitAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilitySpecular_transmit,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToVolumeAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilityVolume);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToVolumeAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilityVolume,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToDiffuseReflectAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilityDiffuse_reflect);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToDiffuseReflectAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilityDiffuse_reflect,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToSpecularReflectAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilitySpecular_reflect);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToSpecularReflectAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilitySpecular_reflect,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiVisibleToSubsurfaceAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiVisibilitySubsurface);
}

UsdAttribute
UsdAiShapeAPI::CreateAiVisibleToSubsurfaceAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiVisibilitySubsurface,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToCameraAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessCamera);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToCameraAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessCamera,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToShadowAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessShadow);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToShadowAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessShadow,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToDiffuseTransmitAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessDiffuse_transmit);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToDiffuseTransmitAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessDiffuse_transmit,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToSpecularTransmitAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessSpecular_transmit);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToSpecularTransmitAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessSpecular_transmit,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToVolumeAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessVolume);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToVolumeAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessVolume,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToDiffuseReflectAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessDiffuse_reflect);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToDiffuseReflectAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessDiffuse_reflect,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToSpecularReflectAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessSpecular_reflect);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToSpecularReflectAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessSpecular_reflect,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDoubleSidedToSubsurfaceAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSidednessSubsurface);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDoubleSidedToSubsurfaceAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSidednessSubsurface,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToCameraAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilityCamera);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToCameraAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilityCamera,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToShadowAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilityShadow);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToShadowAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilityShadow,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToDiffuseTransmitAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilityDiffuse_transmit);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToDiffuseTransmitAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilityDiffuse_transmit,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToSpecularTransmitAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilitySpecular_transmit);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToSpecularTransmitAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilitySpecular_transmit,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToVolumeAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilityVolume);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToVolumeAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilityVolume,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToDiffuseReflectAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilityDiffuse_reflect);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToDiffuseReflectAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilityDiffuse_reflect,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToSpecularReflectAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilitySpecular_reflect);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToSpecularReflectAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilitySpecular_reflect,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiAutobumpVisibleToSubsurfaceAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiAutobump_visibilitySubsurface);
}

UsdAttribute
UsdAiShapeAPI::CreateAiAutobumpVisibleToSubsurfaceAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiAutobump_visibilitySubsurface,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiUseLightGroupAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiUse_light_group);
}

UsdAttribute
UsdAiShapeAPI::CreateAiUseLightGroupAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiUse_light_group,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiUseShadowGroupAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiUse_shadow_group);
}

UsdAttribute
UsdAiShapeAPI::CreateAiUseShadowGroupAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiUse_shadow_group,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSmoothingAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSmoothing);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSmoothingAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSmoothing,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSubdivTypeAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSubdiv_type);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSubdivTypeAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSubdiv_type,
                       SdfValueTypeNames->Token,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSubdivIterationsAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSubdiv_iterations);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSubdivIterationsAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSubdiv_iterations,
                       SdfValueTypeNames->UInt,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSubdivAdaptiveErrorAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSubdiv_adaptive_error);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSubdivAdaptiveErrorAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSubdiv_adaptive_error,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSubdivAdaptiveMetricAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSubdiv_adaptive_metric);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSubdivAdaptiveMetricAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSubdiv_adaptive_metric,
                       SdfValueTypeNames->Token,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSubdivAdaptiveSpaceAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSubdiv_adaptive_space);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSubdivAdaptiveSpaceAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSubdiv_adaptive_space,
                       SdfValueTypeNames->Token,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSubdivUVSmoothingAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSubdiv_uv_smoothing);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSubdivUVSmoothingAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSubdiv_uv_smoothing,
                       SdfValueTypeNames->Token,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiSubdivSmoothDerivsAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiSubdiv_smooth_derivs);
}

UsdAttribute
UsdAiShapeAPI::CreateAiSubdivSmoothDerivsAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiSubdiv_smooth_derivs,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDispPaddingAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiDisp_padding);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDispPaddingAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiDisp_padding,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDispHeightAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiDisp_height);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDispHeightAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiDisp_height,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDispZeroValueAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiDisp_zero_value);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDispZeroValueAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiDisp_zero_value,
                       SdfValueTypeNames->Float,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
UsdAiShapeAPI::GetAiDispAutobumpAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->aiDisp_autobump);
}

UsdAttribute
UsdAiShapeAPI::CreateAiDispAutobumpAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->aiDisp_autobump,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityUniform,
                       defaultValue,
                       writeSparsely);
}

UsdRelationship
UsdAiShapeAPI::GetAiLightGroupRel() const
{
    return GetPrim().GetRelationship(UsdAiTokens->aiLight_group);
}

UsdRelationship
UsdAiShapeAPI::CreateAiLightGroupRel() const
{
    return GetPrim().CreateRelationship(UsdAiTokens->aiLight_group,
                       /* custom = */ false);
}

UsdRelationship
UsdAiShapeAPI::GetAiShadowGroupRel() const
{
    return GetPrim().GetRelationship(UsdAiTokens->aiShadow_group);
}

UsdRelationship
UsdAiShapeAPI::CreateAiShadowGroupRel() const
{
    return GetPrim().CreateRelationship(UsdAiTokens->aiShadow_group,
                       /* custom = */ false);
}

UsdRelationship
UsdAiShapeAPI::GetAiSubdiv_dicing_cameraRel() const
{
    return GetPrim().GetRelationship(UsdAiTokens->aiSubdiv_dicing_camera);
}

UsdRelationship
UsdAiShapeAPI::CreateAiSubdiv_dicing_cameraRel() const
{
    return GetPrim().CreateRelationship(UsdAiTokens->aiSubdiv_dicing_camera,
                       /* custom = */ false);
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
        UsdAiTokens->aiOpaque,
        UsdAiTokens->aiMatte,
        UsdAiTokens->aiRay_bias,
        UsdAiTokens->aiReceive_shadows,
        UsdAiTokens->aiSelf_shadows,
        UsdAiTokens->aiTransform_type,
        UsdAiTokens->aiVisibilityCamera,
        UsdAiTokens->aiVisibilityShadow,
        UsdAiTokens->aiVisibilityDiffuse_transmit,
        UsdAiTokens->aiVisibilitySpecular_transmit,
        UsdAiTokens->aiVisibilityVolume,
        UsdAiTokens->aiVisibilityDiffuse_reflect,
        UsdAiTokens->aiVisibilitySpecular_reflect,
        UsdAiTokens->aiVisibilitySubsurface,
        UsdAiTokens->aiSidednessCamera,
        UsdAiTokens->aiSidednessShadow,
        UsdAiTokens->aiSidednessDiffuse_transmit,
        UsdAiTokens->aiSidednessSpecular_transmit,
        UsdAiTokens->aiSidednessVolume,
        UsdAiTokens->aiSidednessDiffuse_reflect,
        UsdAiTokens->aiSidednessSpecular_reflect,
        UsdAiTokens->aiSidednessSubsurface,
        UsdAiTokens->aiAutobump_visibilityCamera,
        UsdAiTokens->aiAutobump_visibilityShadow,
        UsdAiTokens->aiAutobump_visibilityDiffuse_transmit,
        UsdAiTokens->aiAutobump_visibilitySpecular_transmit,
        UsdAiTokens->aiAutobump_visibilityVolume,
        UsdAiTokens->aiAutobump_visibilityDiffuse_reflect,
        UsdAiTokens->aiAutobump_visibilitySpecular_reflect,
        UsdAiTokens->aiAutobump_visibilitySubsurface,
        UsdAiTokens->aiUse_light_group,
        UsdAiTokens->aiUse_shadow_group,
        UsdAiTokens->aiSmoothing,
        UsdAiTokens->aiSubdiv_type,
        UsdAiTokens->aiSubdiv_iterations,
        UsdAiTokens->aiSubdiv_adaptive_error,
        UsdAiTokens->aiSubdiv_adaptive_metric,
        UsdAiTokens->aiSubdiv_adaptive_space,
        UsdAiTokens->aiSubdiv_uv_smoothing,
        UsdAiTokens->aiSubdiv_smooth_derivs,
        UsdAiTokens->aiDisp_padding,
        UsdAiTokens->aiDisp_height,
        UsdAiTokens->aiDisp_zero_value,
        UsdAiTokens->aiDisp_autobump,
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

PXR_NAMESPACE_OPEN_SCOPE

#include <ai_ray.h>

namespace {
    using _MaskQuery = std::pair<decltype(&UsdAiShapeAPI::GetAiVisibleToCameraAttr), uint8_t>;
    using _MaskQueryVector = std::vector<_MaskQuery>;

    inline
    uint8_t _getMask(
        const UsdAiShapeAPI& api,
        const _MaskQueryVector& l) {
        uint8_t result = 0;
        for (const auto& each: l) {
            if (((api).*(each.first))()) {
                result |= each.second;
            }
        }
        return result;
    }
}

uint8_t
UsdAiShapeAPI::ComputeVisibility() const {
    static const _MaskQueryVector _query = {
        {&UsdAiShapeAPI::GetAiVisibleToCameraAttr, AI_RAY_CAMERA},
        {&UsdAiShapeAPI::GetAiVisibleToShadowAttr, AI_RAY_SHADOW},
        {&UsdAiShapeAPI::GetAiVisibleToDiffuseTransmitAttr, AI_RAY_DIFFUSE_TRANSMIT},
        {&UsdAiShapeAPI::GetAiVisibleToSpecularTransmitAttr, AI_RAY_SPECULAR_TRANSMIT},
        {&UsdAiShapeAPI::GetAiVisibleToVolumeAttr, AI_RAY_VOLUME},
        {&UsdAiShapeAPI::GetAiVisibleToDiffuseReflectAttr, AI_RAY_DIFFUSE_REFLECT},
        {&UsdAiShapeAPI::GetAiVisibleToSpecularReflectAttr, AI_RAY_SPECULAR_REFLECT},
        {&UsdAiShapeAPI::GetAiVisibleToSubsurfaceAttr, AI_RAY_SUBSURFACE},
    };
    return _getMask(*this, _query);
}

uint8_t
UsdAiShapeAPI::ComputeSidedness() const {
    static const _MaskQueryVector _query = {
        {&UsdAiShapeAPI::GetAiDoubleSidedToCameraAttr, AI_RAY_CAMERA},
        {&UsdAiShapeAPI::GetAiDoubleSidedToShadowAttr, AI_RAY_SHADOW},
        {&UsdAiShapeAPI::GetAiDoubleSidedToDiffuseTransmitAttr, AI_RAY_DIFFUSE_TRANSMIT},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSpecularTransmitAttr, AI_RAY_SPECULAR_TRANSMIT},
        {&UsdAiShapeAPI::GetAiDoubleSidedToVolumeAttr, AI_RAY_VOLUME},
        {&UsdAiShapeAPI::GetAiDoubleSidedToDiffuseReflectAttr, AI_RAY_DIFFUSE_REFLECT},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSpecularReflectAttr, AI_RAY_SPECULAR_REFLECT},
        {&UsdAiShapeAPI::GetAiDoubleSidedToSubsurfaceAttr, AI_RAY_SUBSURFACE},
    };
    return _getMask(*this, _query);
}

uint8_t
UsdAiShapeAPI::ComputeAutobumpVisibility() const {
    static const _MaskQueryVector _query = {
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToCameraAttr, AI_RAY_CAMERA},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToShadowAttr, AI_RAY_SHADOW},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToDiffuseTransmitAttr, AI_RAY_DIFFUSE_TRANSMIT},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToSpecularTransmitAttr, AI_RAY_SPECULAR_TRANSMIT},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToVolumeAttr, AI_RAY_VOLUME},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToDiffuseReflectAttr, AI_RAY_DIFFUSE_REFLECT},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToSpecularReflectAttr, AI_RAY_SPECULAR_REFLECT},
        {&UsdAiShapeAPI::GetAiAutobumpVisibleToSubsurfaceAttr, AI_RAY_SUBSURFACE},
    };
    return _getMask(*this, _query);
}

PXR_NAMESPACE_CLOSE_SCOPE
