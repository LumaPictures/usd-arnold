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

#include "pxr/usd/usdAi/rayTypes4.h"
#include "pxr/usd/usdAi/rayTypes5.h"


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
/// In case of ray types, there is a difference between the 4th
/// and 5th major release of Arnold. We provide both in their
/// respective namespace. In case arnold headers are not present
/// just use the provided macros.
/// USD_ARNOLD_IMPORT_ARNOLD4 pr USD_ARNOLD_IMPORT_ARNOLD5
/// 
/// For Arnold 4
/// AI_RAY_UNDEFINED   undefined type
/// AI_RAY_CAMERA      ray originating at the camera
/// AI_RAY_SHADOW      shadow ray towards a light source
/// AI_RAY_REFLECTED   mirror reflection ray
/// AI_RAY_REFRACTED   mirror refraction ray
/// AI_RAY_SUBSURFACE  subsurface scattering probe ray
/// AI_RAY_DIFFUSE     indirect diffuse (also known as diffuse GI) ray
/// AI_RAY_GLOSSY      glossy/blurred reflection ray
/// AI_RAY_ALL         mask for all ray types
/// AI_RAY_GENERIC     mask for all ray types
/// 
/// For Arnold 5
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
    // OPAQUE 
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
    UsdAttribute GetOpaqueAttr() const;

    /// See GetOpaqueAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateOpaqueAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // MATTE 
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
    UsdAttribute GetMatteAttr() const;

    /// See GetMatteAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateMatteAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // RECEIVESHADOWS 
    // --------------------------------------------------------------------- //
    /// Whether the object picks up shadows from other objects.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetReceiveShadowsAttr() const;

    /// See GetReceiveShadowsAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateReceiveShadowsAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // SELFSHADOWS 
    // --------------------------------------------------------------------- //
    /// Whether the object casts shadows on itself.
    ///
    /// \n  C++ Type: bool
    /// \n  Usd Type: SdfValueTypeNames->Bool
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: True
    USDAI_API
    UsdAttribute GetSelfShadowsAttr() const;

    /// See GetSelfShadowsAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateSelfShadowsAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // VISIBILITY 
    // --------------------------------------------------------------------- //
    /// The visibility of the object for each Arnold ray type, as a
    /// bitmask.
    /// 
    /// You can selectively disable an object's visibility for the
    /// various types of rays in the renderer. By default, objects are
    /// visible to all types of rays.
    ///
    /// \n  C++ Type: unsigned char
    /// \n  Usd Type: SdfValueTypeNames->UChar
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 255
    USDAI_API
    UsdAttribute GetVisibilityAttr() const;

    /// See GetVisibilityAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateVisibilityAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

public:
    // --------------------------------------------------------------------- //
    // SIDEDNESS 
    // --------------------------------------------------------------------- //
    /// The double-sidedness of the object for each Arnold ray type, as
    /// a bitmask.
    /// 
    /// Just as you can disable an object's visibility for specific ray
    /// types, you can also change its sidedness. By default, objects
    /// are double-sided for all rays.
    ///
    /// \n  C++ Type: unsigned char
    /// \n  Usd Type: SdfValueTypeNames->UChar
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: 255
    USDAI_API
    UsdAttribute GetSidednessAttr() const;

    /// See GetSidednessAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateSidednessAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

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
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
