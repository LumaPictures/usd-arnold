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
#ifndef USDAI_TOKENS_H
#define USDAI_TOKENS_H

/// \file usdAi/tokens.h

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// 
// This is an automatically generated file (by usdGenSchema.py).
// Do not hand-edit!
// 
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "pxr/pxr.h"
#include "pxr/usd/usdAi/api.h"
#include "pxr/base/tf/staticData.h"
#include "pxr/base/tf/token.h"
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE


/// \class UsdAiTokensType
///
/// \link UsdAiTokens \endlink provides static, efficient
/// \link TfToken TfTokens\endlink for use in all public USD API.
///
/// These tokens are auto-generated from the module's schema, representing
/// property names, for when you need to fetch an attribute or relationship
/// directly by name, e.g. UsdPrim::GetAttribute(), in the most efficient
/// manner, and allow the compiler to verify that you spelled the name
/// correctly.
///
/// UsdAiTokens also contains all of the \em allowedTokens values
/// declared for schema builtin attributes of 'token' scene description type.
/// Use UsdAiTokens like so:
///
/// \code
///     gprim.GetMyTokenValuedAttr().Set(UsdAiTokens->aiAov);
/// \endcode
struct UsdAiTokensType {
    USDAI_API UsdAiTokensType();
    /// \brief "ai:aov"
    /// 
    /// UsdAiLightAPI
    const TfToken aiAov;
    /// \brief "ai:autobump_visibility:camera"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilityCamera;
    /// \brief "ai:autobump_visibility:diffuse_reflect"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilityDiffuse_reflect;
    /// \brief "ai:autobump_visibility:diffuse_transmit"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilityDiffuse_transmit;
    /// \brief "ai:autobump_visibility:shadow"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilityShadow;
    /// \brief "ai:autobump_visibility:specular_reflect"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilitySpecular_reflect;
    /// \brief "ai:autobump_visibility:specular_transmit"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilitySpecular_transmit;
    /// \brief "ai:autobump_visibility:subsurface"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilitySubsurface;
    /// \brief "ai:autobump_visibility:volume"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiAutobump_visibilityVolume;
    /// \brief "ai:disp_autobump"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiDisp_autobump;
    /// \brief "ai:disp_height"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiDisp_height;
    /// \brief "ai:disp_padding"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiDisp_padding;
    /// \brief "ai:disp_zero_value"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiDisp_zero_value;
    /// \brief "ai:displacement"
    /// 
    /// UsdAiMaterialAPI
    const TfToken aiDisplacement;
    /// \brief "ai:light_group"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiLight_group;
    /// \brief "ai:matte"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiMatte;
    /// \brief "ai:opaque"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiOpaque;
    /// \brief "ai:ray_bias"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiRay_bias;
    /// \brief "ai:receive_shadows"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiReceive_shadows;
    /// \brief "ai:self_shadows"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSelf_shadows;
    /// \brief "ai:shadow_group"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiShadow_group;
    /// \brief "ai:sidedness:camera"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessCamera;
    /// \brief "ai:sidedness:diffuse_reflect"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessDiffuse_reflect;
    /// \brief "ai:sidedness:diffuse_transmit"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessDiffuse_transmit;
    /// \brief "ai:sidedness:shadow"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessShadow;
    /// \brief "ai:sidedness:specular_reflect"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessSpecular_reflect;
    /// \brief "ai:sidedness:specular_transmit"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessSpecular_transmit;
    /// \brief "ai:sidedness:subsurface"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessSubsurface;
    /// \brief "ai:sidedness:volume"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSidednessVolume;
    /// \brief "ai:smoothing"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSmoothing;
    /// \brief "ai:subdiv_adaptive_error"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_adaptive_error;
    /// \brief "ai:subdiv_adaptive_metric"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_adaptive_metric;
    /// \brief "ai:subdiv_adaptive_space"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_adaptive_space;
    /// \brief "ai:subdiv_dicing_camera"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_dicing_camera;
    /// \brief "ai:subdiv_iterations"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_iterations;
    /// \brief "ai:subdiv_smooth_derivs"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_smooth_derivs;
    /// \brief "ai:subdiv_type"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_type;
    /// \brief "ai:subdiv_uv_smoothing"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiSubdiv_uv_smoothing;
    /// \brief "ai:surface"
    /// 
    /// UsdAiMaterialAPI
    const TfToken aiSurface;
    /// \brief "ai:transform_type"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiTransform_type;
    /// \brief "ai:use_light_group"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiUse_light_group;
    /// \brief "ai:use_shadow_group"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiUse_shadow_group;
    /// \brief "ai:visibility:camera"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilityCamera;
    /// \brief "ai:visibility:diffuse_reflect"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilityDiffuse_reflect;
    /// \brief "ai:visibility:diffuse_transmit"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilityDiffuse_transmit;
    /// \brief "ai:visibility:shadow"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilityShadow;
    /// \brief "ai:visibility:specular_reflect"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilitySpecular_reflect;
    /// \brief "ai:visibility:specular_transmit"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilitySpecular_transmit;
    /// \brief "ai:visibility:subsurface"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilitySubsurface;
    /// \brief "ai:visibility:volume"
    /// 
    /// UsdAiShapeAPI
    const TfToken aiVisibilityVolume;
    /// \brief "ARRAY"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken aRRAY;
    /// \brief "auto_"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_adaptive_metricAttr(), Default value for UsdAiShapeAPI::GetAiSubdiv_adaptive_metricAttr()
    const TfToken auto_;
    /// \brief "BOOL"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken bOOL;
    /// \brief "BYTE"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken bYTE;
    /// \brief "catclark"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_typeAttr()
    const TfToken catclark;
    /// \brief "data"
    /// 
    /// UsdAiProcedural
    const TfToken data;
    /// \brief "dataType"
    /// 
    /// UsdAiAOV
    const TfToken dataType;
    /// \brief "driver"
    /// 
    /// UsdAiAOV
    const TfToken driver;
    /// \brief "edge_length"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_adaptive_metricAttr()
    const TfToken edge_length;
    /// \brief "filename"
    /// 
    /// UsdAiVolume
    const TfToken filename;
    /// \brief "filter"
    /// 
    /// UsdAiAOV
    const TfToken filter;
    /// \brief "flatness"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_adaptive_metricAttr()
    const TfToken flatness;
    /// \brief "FLOAT"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken fLOAT;
    /// \brief "info:id"
    /// 
    /// UsdAiProcedural
    const TfToken infoId;
    /// \brief "info:node_entry_type"
    /// 
    /// UsdAiNodeAPI
    const TfToken infoNode_entry_type;
    /// \brief "INT"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken iNT;
    /// \brief "linear"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_typeAttr(), Possible value for UsdAiShapeAPI::GetAiTransform_typeAttr(), Possible value for UsdAiShapeAPI::GetAiSubdiv_uv_smoothingAttr()
    const TfToken linear;
    /// \brief "LPE"
    /// 
    /// UsdAiAOV
    const TfToken lPE;
    /// \brief "MATRIX"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken mATRIX;
    /// \brief "name"
    /// 
    /// UsdAiAOV
    const TfToken name;
    /// \brief "NODE"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken nODE;
    /// \brief "none"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_typeAttr(), Default value for UsdAiShapeAPI::GetAiSubdiv_typeAttr()
    const TfToken none;
    /// \brief "object"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_adaptive_spaceAttr()
    const TfToken object;
    /// \brief "paramType"
    /// 
    /// Stores the arnold parameter type for a parameter when remapping is non-trivial.
    const TfToken paramType;
    /// \brief "path"
    /// 
    /// UsdAiDriver
    const TfToken path;
    /// \brief "pin_borders"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_uv_smoothingAttr()
    const TfToken pin_borders;
    /// \brief "pin_corners"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_uv_smoothingAttr(), Default value for UsdAiShapeAPI::GetAiSubdiv_uv_smoothingAttr()
    const TfToken pin_corners;
    /// \brief "POINTER"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken pOINTER;
    /// \brief "raster"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_adaptive_spaceAttr(), Default value for UsdAiShapeAPI::GetAiSubdiv_adaptive_spaceAttr()
    const TfToken raster;
    /// \brief "RGB"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken rGB;
    /// \brief "RGBA"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr(), Default value for UsdAiAOV::GetDataTypeAttr()
    const TfToken rGBA;
    /// \brief "rotate_about_center"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiTransform_typeAttr(), Default value for UsdAiShapeAPI::GetAiTransform_typeAttr()
    const TfToken rotate_about_center;
    /// \brief "rotate_about_origin"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiTransform_typeAttr()
    const TfToken rotate_about_origin;
    /// \brief "size"
    /// 
    /// UsdAiFilter
    const TfToken size;
    /// \brief "smooth"
    /// 
    /// Possible value for UsdAiShapeAPI::GetAiSubdiv_uv_smoothingAttr()
    const TfToken smooth;
    /// \brief "step_size"
    /// 
    /// UsdAiVolume, UsdAiVolumeProcedural
    const TfToken step_size;
    /// \brief "UINT"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken uINT;
    /// \brief "user:"
    /// 
    /// The attribute prefix used to qualify user parameters on nodes using the AiNodeAPI.
    const TfToken userPrefix;
    /// \brief "VECTOR"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken vECTOR;
    /// \brief "VECTOR2"
    /// 
    /// Possible value for UsdAiAOV::GetDataTypeAttr()
    const TfToken vECTOR2;
    /// A vector of all of the tokens listed above.
    const std::vector<TfToken> allTokens;
};

/// \var UsdAiTokens
///
/// A global variable with static, efficient \link TfToken TfTokens\endlink
/// for use in all public USD API.  \sa UsdAiTokensType
extern USDAI_API TfStaticData<UsdAiTokensType> UsdAiTokens;

PXR_NAMESPACE_CLOSE_SCOPE

#endif
