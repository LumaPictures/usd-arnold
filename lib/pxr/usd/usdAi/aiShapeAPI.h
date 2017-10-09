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
#ifndef USDAI_GENERATED_AISHAPEAPI_H
#define USDAI_GENERATED_AISHAPEAPI_H

/// \file usdAi/aiShapeAPI.h

#include "pxr/pxr.h"
#include "pxr/usd/usdAi/api.h"
#include "pxr/usd/usd/schemaBase.h"
#include "pxr/usd/usd/prim.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usdAi/tokens.h"

#include "pxr/usd/usdAi/rayTypes.h"


#include "pxr/base/vt/value.h"

#include "pxr/base/gf/vec3d.h"
#include "pxr/base/gf/vec3f.h"
#include "pxr/base/gf/matrix4d.h"

#include "pxr/base/tf/token.h"
#include "pxr/base/tf/type.h"

PXR_NAMESPACE_OPEN_SCOPE

class SdfAssetPath;

// -------------------------------------------------------------------------- //
// AISHAPEAPI                                                                 //
// -------------------------------------------------------------------------- //

/// \class UsdAiShapeAPI
///
/// API for interacting with common Arnold shape parameters on prims.
/// 
/// Currently handles:
/// - The bitmasks `visibility` and `sidedness`
/// - The boolean flags `matte`, `opaque`, `self_shadows`, and
/// `receive_shadows`.
/// 
/// To access the ray types without installing arnold,
/// include rayTypes.h and use the namespace arnold.
/// 
/// Arnold 5 ray types
/// AI_RAY_UNDEFINED         undefined type
/// AI_RAY_CAMERA            ray originating at the camera
/// AI_RAY_SHADOW            shadow ray towards a light source
/// AI_RAY_DIFFUSE_TRANSMIT  indirect diffuse transmission ray
/// AI_RAY_SPECULAR_TRANSMIT indirect specular transmission ray
/// AI_RAY_VOLUME            indirect volume scattering ray
/// AI_RAY_DIFFUSE_REFLECT   indirect diffuse reflection ray
/// AI_RAY_SPECULAR_REFLECT  indirect specular reflection ray
/// AI_RAY_SUBSURFACE        subsurface scattering probe ray
/// 
/// AI_RAY_ALL_DIFFUSE  all indirect diffuse ray types
/// AI_RAY_ALL_SPECULAR all indirect specular ray types
/// AI_RAY_ALL_REFLECT  all reflection ray types
/// AI_RAY_ALL_TRANSMIT all transmission specular ray types
/// AI_RAY_ALL          mask for all ray types
///
/// For any described attribute \em Fallback \em Value or \em Allowed \em Values below
/// that are text/tokens, the actual token is published and defined in \ref UsdAiTokens.
/// So to set an attribute to the value "rightHanded", use UsdAiTokens->rightHanded
/// as the value.
///
class UsdAiShapeAPI : public UsdSchemaBase
{
public:
    /// Compile-time constant indicating whether or not this class corresponds
    /// to a concrete instantiable prim type in scene description.  If this is
    /// true, GetStaticPrimDefinition() will return a valid prim definition with
    /// a non-empty typeName.
    static const bool IsConcrete = false;

    /// Construct a UsdAiShapeAPI on UsdPrim \p prim .
    /// Equivalent to UsdAiShapeAPI::Get(prim.GetStage(), prim.GetPath())
    /// for a \em valid \p prim, but will not immediately throw an error for
    /// an invalid \p prim
    explicit UsdAiShapeAPI(const UsdPrim& prim=UsdPrim())
        : UsdSchemaBase(prim)
    {
    }

    /// Construct a UsdAiShapeAPI on the prim held by \p schemaObj .
    /// Should be preferred over UsdAiShapeAPI(schemaObj.GetPrim()),
    /// as it preserves SchemaBase state.
    explicit UsdAiShapeAPI(const UsdSchemaBase& schemaObj)
        : UsdSchemaBase(schemaObj)
    {
    }

    /// Destructor.
    USDAI_API
    virtual ~UsdAiShapeAPI();

    /// Return a vector of names of all pre-declared attributes for this schema
    /// class and all its ancestor classes.  Does not include attributes that
    /// may be authored by custom/extended methods of the schemas involved.
    USDAI_API
    static const TfTokenVector &
    GetSchemaAttributeNames(bool includeInherited=true);

    /// Return a UsdAiShapeAPI holding the prim adhering to this
    /// schema at \p path on \p stage.  If no prim exists at \p path on
    /// \p stage, or if the prim at that path does not adhere to this schema,
    /// return an invalid schema object.  This is shorthand for the following:
    ///
    /// \code
    /// UsdAiShapeAPI(stage->GetPrimAtPath(path));
    /// \endcode
    ///
    USDAI_API
    static UsdAiShapeAPI
    Get(const UsdStagePtr &stage, const SdfPath &path);


private:
    // needs to invoke _GetStaticTfType.
    friend class UsdSchemaRegistry;
    USDAI_API
    static const TfType &_GetStaticTfType();

    static bool _IsTypedSchema();

    // override SchemaBase virtuals.
    USDAI_API
    virtual const TfType &_GetTfType() const;

public:
    // --------------------------------------------------------------------- //
    // AIOPAQUE 
    // --------------------------------------------------------------------- //
    /// Whether the object should be considered fully opaque. By
    /// default all objects are marked as opaque, because it speeds up
    /// rendering considerably.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiOpaqueAttr() const;

    /// See GetAiOpaqueAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiOpaqueAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIMATTE 
    // --------------------------------------------------------------------- //
    /// This attribute, when enabled, turns an object or group into a
    /// holdout matte. This affects primary rays only.
    /// 
    /// The matte color is always black, and the opacity of the object
    /// is preserved. Shaders will not run on the object, and it will
    /// output all black (including the alpha), except if the opaque
    /// flag is off, in which case shaders will be run to compute the
    /// opacity only. Note that even AOVs output by its shaders in this
    /// case will be black.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiMatteAttr() const;

    /// See GetAiMatteAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiMatteAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIRECEIVESHADOWS 
    // --------------------------------------------------------------------- //
    /// Whether the object picks up shadows from other objects.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiReceiveShadowsAttr() const;

    /// See GetAiReceiveShadowsAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiReceiveShadowsAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIRAYBIAS 
    // --------------------------------------------------------------------- //
    /// Ray bias for handling precision issues.
    ///
    /// \n  C++ Type: float
    /// \n  Usd Type: SdfValueTypeNames->Float
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 9.99999997475e-07
    USDAI_API
    UsdAttribute GetAiRayBiasAttr() const;

    /// See GetAiRayBiasAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiRayBiasAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISELFSHADOWS 
    // --------------------------------------------------------------------- //
    /// Whether the object casts shadows on itself.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiSelfShadowsAttr() const;

    /// See GetAiSelfShadowsAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSelfShadowsAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETOCAMERA 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to camera rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToCameraAttr() const;

    /// See GetAiVisibleToCameraAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToCameraAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETOSHADOW 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to shadow rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToShadowAttr() const;

    /// See GetAiVisibleToShadowAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToShadowAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETODIFFUSETRANSMIT 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to diffuse transmission rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToDiffuseTransmitAttr() const;

    /// See GetAiVisibleToDiffuseTransmitAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToDiffuseTransmitAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETOSPECULARTRANSMIT 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to specular transmission rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToSpecularTransmitAttr() const;

    /// See GetAiVisibleToSpecularTransmitAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToSpecularTransmitAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETOVOLUME 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to volume scattering rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToVolumeAttr() const;

    /// See GetAiVisibleToVolumeAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToVolumeAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETODIFFUSEREFLECT 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to diffuse reflection rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToDiffuseReflectAttr() const;

    /// See GetAiVisibleToDiffuseReflectAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToDiffuseReflectAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETOSPECULARREFLECT 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to specular reflection rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToSpecularReflectAttr() const;

    /// See GetAiVisibleToSpecularReflectAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToSpecularReflectAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIVISIBLETOSUBSURFACE 
    // --------------------------------------------------------------------- //
    /// Whether the object is visible to subsurface scattering rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiVisibleToSubsurfaceAttr() const;

    /// See GetAiVisibleToSubsurfaceAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiVisibleToSubsurfaceAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTOCAMERA 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to camera rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToCameraAttr() const;

    /// See GetAiDoubleSidedToCameraAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToCameraAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTOSHADOW 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to shadow rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToShadowAttr() const;

    /// See GetAiDoubleSidedToShadowAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToShadowAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTODIFFUSETRANSMIT 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to diffuse transmission rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToDiffuseTransmitAttr() const;

    /// See GetAiDoubleSidedToDiffuseTransmitAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToDiffuseTransmitAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTOSPECULARTRANSMIT 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to specular transmission rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToSpecularTransmitAttr() const;

    /// See GetAiDoubleSidedToSpecularTransmitAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToSpecularTransmitAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTOVOLUME 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to volume scattering rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToVolumeAttr() const;

    /// See GetAiDoubleSidedToVolumeAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToVolumeAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTODIFFUSEREFLECT 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to diffuse reflection rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToDiffuseReflectAttr() const;

    /// See GetAiDoubleSidedToDiffuseReflectAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToDiffuseReflectAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTOSPECULARREFLECT 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to specular reflection rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToSpecularReflectAttr() const;

    /// See GetAiDoubleSidedToSpecularReflectAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToSpecularReflectAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDOUBLESIDEDTOSUBSURFACE 
    // --------------------------------------------------------------------- //
    /// Whether the object is double-sided to subsurface scattering rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiDoubleSidedToSubsurfaceAttr() const;

    /// See GetAiDoubleSidedToSubsurfaceAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDoubleSidedToSubsurfaceAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETOCAMERA 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for camera rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToCameraAttr() const;

    /// See GetAiAutobumpVisibleToCameraAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToCameraAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETOSHADOW 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for shadow rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToShadowAttr() const;

    /// See GetAiAutobumpVisibleToShadowAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToShadowAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETODIFFUSETRANSMIT 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for diffuse transmission rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToDiffuseTransmitAttr() const;

    /// See GetAiAutobumpVisibleToDiffuseTransmitAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToDiffuseTransmitAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETOSPECULARTRANSMIT 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for specular transmission rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToSpecularTransmitAttr() const;

    /// See GetAiAutobumpVisibleToSpecularTransmitAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToSpecularTransmitAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETOVOLUME 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for volume scattering rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToVolumeAttr() const;

    /// See GetAiAutobumpVisibleToVolumeAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToVolumeAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETODIFFUSEREFLECT 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for diffuse reflection rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToDiffuseReflectAttr() const;

    /// See GetAiAutobumpVisibleToDiffuseReflectAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToDiffuseReflectAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETOSPECULARREFLECT 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for specular reflection rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToSpecularReflectAttr() const;

    /// See GetAiAutobumpVisibleToSpecularReflectAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToSpecularReflectAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIAUTOBUMPVISIBLETOSUBSURFACE 
    // --------------------------------------------------------------------- //
    /// Whether the autobump is enabled for subsurface scattering rays.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiAutobumpVisibleToSubsurfaceAttr() const;

    /// See GetAiAutobumpVisibleToSubsurfaceAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiAutobumpVisibleToSubsurfaceAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIUSELIGHTGROUP 
    // --------------------------------------------------------------------- //
    /// Enable the use of light groups.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiUseLightGroupAttr() const;

    /// See GetAiUseLightGroupAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiUseLightGroupAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIUSESHADOWGROUP 
    // --------------------------------------------------------------------- //
    /// Enable the use of shadow groups.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiUseShadowGroupAttr() const;

    /// See GetAiUseShadowGroupAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiUseShadowGroupAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISMOOTHING 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiSmoothingAttr() const;

    /// See GetAiSmoothingAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSmoothingAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIVTYPE 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: TfToken
    /// \n  Usd Type: SdfValueTypeNames->Token
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: none
    /// \n  \ref UsdAiTokens "Allowed Values": [none, catclark, linear]
    USDAI_API
    UsdAttribute GetAiSubdivTypeAttr() const;

    /// See GetAiSubdivTypeAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSubdivTypeAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIVITERATIONS 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: unsigned int
    /// \n  Usd Type: SdfValueTypeNames->UInt
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 1
    USDAI_API
    UsdAttribute GetAiSubdivIterationsAttr() const;

    /// See GetAiSubdivIterationsAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSubdivIterationsAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIVADAPTIVEERROR 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: float
    /// \n  Usd Type: SdfValueTypeNames->Float
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 0.0
    USDAI_API
    UsdAttribute GetAiSubdivAdaptiveErrorAttr() const;

    /// See GetAiSubdivAdaptiveErrorAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSubdivAdaptiveErrorAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIVADAPTIVEMETRIC 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: TfToken
    /// \n  Usd Type: SdfValueTypeNames->Token
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: auto_
    /// \n  \ref UsdAiTokens "Allowed Values": [auto_, edge_length, flatness]
    USDAI_API
    UsdAttribute GetAiSubdivAdaptiveMetricAttr() const;

    /// See GetAiSubdivAdaptiveMetricAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSubdivAdaptiveMetricAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIVADAPTIVESPACE 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: TfToken
    /// \n  Usd Type: SdfValueTypeNames->Token
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: raster
    /// \n  \ref UsdAiTokens "Allowed Values": [raster, object]
    USDAI_API
    UsdAttribute GetAiSubdivAdaptiveSpaceAttr() const;

    /// See GetAiSubdivAdaptiveSpaceAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSubdivAdaptiveSpaceAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIVUVSMOOTHING 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: TfToken
    /// \n  Usd Type: SdfValueTypeNames->Token
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: pin_corners
    /// \n  \ref UsdAiTokens "Allowed Values": [pin_corners, pin_borders, linear, smooth]
    USDAI_API
    UsdAttribute GetAiSubdivUVSmoothingAttr() const;

    /// See GetAiSubdivUVSmoothingAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSubdivUVSmoothingAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIVSMOOTHDERIVS 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiSubdivSmoothDerivsAttr() const;

    /// See GetAiSubdivSmoothDerivsAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiSubdivSmoothDerivsAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDISPPADDING 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: float
    /// \n  Usd Type: SdfValueTypeNames->Float
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 0.0
    USDAI_API
    UsdAttribute GetAiDispPaddingAttr() const;

    /// See GetAiDispPaddingAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDispPaddingAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDISPHEIGHT 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: float
    /// \n  Usd Type: SdfValueTypeNames->Float
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 1.0
    USDAI_API
    UsdAttribute GetAiDispHeightAttr() const;

    /// See GetAiDispHeightAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDispHeightAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDISPZEROVALUE 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: float
    /// \n  Usd Type: SdfValueTypeNames->Float
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 0.0
    USDAI_API
    UsdAttribute GetAiDispZeroValueAttr() const;

    /// See GetAiDispZeroValueAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDispZeroValueAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AIDISPAUTOBUMP 
    // --------------------------------------------------------------------- //
    /// 
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: False
    USDAI_API
    UsdAttribute GetAiDispAutobumpAttr() const;

    /// See GetAiDispAutobumpAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateAiDispAutobumpAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // AILIGHTGROUP 
    // --------------------------------------------------------------------- //
    /// Light groups for the shape.
    ///
    USDAI_API
    UsdRelationship GetAiLightGroupRel() const;

    /// See GetAiLightGroupRel(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create
    USDAI_API
    UsdRelationship CreateAiLightGroupRel() const;

public:
    // --------------------------------------------------------------------- //
    // AISHADOWGROUP 
    // --------------------------------------------------------------------- //
    /// Shadow groups for the shape.
    ///
    USDAI_API
    UsdRelationship GetAiShadowGroupRel() const;

    /// See GetAiShadowGroupRel(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create
    USDAI_API
    UsdRelationship CreateAiShadowGroupRel() const;

public:
    // --------------------------------------------------------------------- //
    // AISUBDIV_DICING_CAMERA 
    // --------------------------------------------------------------------- //
    /// 
    ///
    USDAI_API
    UsdRelationship GetAiSubdiv_dicing_cameraRel() const;

    /// See GetAiSubdiv_dicing_cameraRel(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create
    USDAI_API
    UsdRelationship CreateAiSubdiv_dicing_cameraRel() const;

public:
    // ===================================================================== //
    // Feel free to add custom code below this line, it will be preserved by 
    // the code generator. 
    //
    // Just remember to: 
    //  - Close the class declaration with }; 
    //  - Close the namespace with PXR_NAMESPACE_CLOSE_SCOPE
    //  - Close the include guard with #endif
    // ===================================================================== //
    // --(BEGIN CUSTOM CODE)--

    /// Computes the visibility bitmask for the shape.
    ///
    USDAI_API
    uint8_t ComputeVisibility() const;

    /// Computes the sidedness bitmask for the shape.
    ///
    USDAI_API
    uint8_t ComputeSidedness() const;

    /// Computes the autobump-visibility bitmask for the shape.
    ///
    USDAI_API
    uint8_t ComputeAutobumpVisibility() const;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
