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
#include "pxr/base/tf/staticTokens.h"

PXR_NAMESPACE_OPEN_SCOPE

/// \hideinitializer
#define USDAI_TOKENS \
    ((aiDisplacement, "ai:displacement")) \
    ((aiSurface, "ai:surface")) \
    (data) \
    (dso) \
    (loat_at_init) \
    (matte) \
    (opaque) \
    (receive_shadows) \
    (self_shadows) \
    (sidedness) \
    (step_size) \
    ((userPrefix, "user:")) \
    (visibility)

/// \anchor UsdAiTokens
///
/// <b>UsdAiTokens</b> provides static, efficient TfToken's for
/// use in all public USD API
///
/// These tokens are auto-generated from the module's schema, representing
/// property names, for when you need to fetch an attribute or relationship
/// directly by name, e.g. UsdPrim::GetAttribute(), in the most efficient
/// manner, and allow the compiler to verify that you spelled the name
/// correctly.
///
/// UsdAiTokens also contains all of the \em allowedTokens values declared
/// for schema builtin attributes of 'token' scene description type.
/// Use UsdAiTokens like so:
///
/// \code
///     gprim.GetVisibilityAttr().Set(UsdAiTokens->invisible);
/// \endcode
///
/// The tokens are:
/// \li <b>aiDisplacement</b> - UsdAiMaterialAPI
/// \li <b>aiSurface</b> - UsdAiMaterialAPI
/// \li <b>data</b> - UsdAiProcedural
/// \li <b>dso</b> - UsdAiProcedural
/// \li <b>loat_at_init</b> - UsdAiProcedural
/// \li <b>matte</b> - UsdAiShapeAPI
/// \li <b>opaque</b> - UsdAiShapeAPI
/// \li <b>receive_shadows</b> - UsdAiShapeAPI
/// \li <b>self_shadows</b> - UsdAiShapeAPI
/// \li <b>sidedness</b> - UsdAiShapeAPI
/// \li <b>step_size</b> - UsdAiVolume
/// \li <b>userPrefix</b> - The attribute prefix used to qualify user parameters on nodes using the AiNodeAPI.
/// \li <b>visibility</b> - UsdAiShapeAPI
TF_DECLARE_PUBLIC_TOKENS(UsdAiTokens, USDAI_API, USDAI_TOKENS);

PXR_NAMESPACE_CLOSE_SCOPE

#endif
