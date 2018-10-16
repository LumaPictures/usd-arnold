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
#ifndef USDAI_GENERATED_AINODEAPI_H
#define USDAI_GENERATED_AINODEAPI_H

/// \file usdAi/aiNodeAPI.h

#include "pxr/pxr.h"
#include "pxr/usd/usdAi/api.h"
#include "pxr/usd/usd/apiSchemaBase.h"
#include "pxr/usd/usd/prim.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usdAi/tokens.h"

struct AtMetaDataEntry;

#include "pxr/base/vt/value.h"

#include "pxr/base/gf/vec3d.h"
#include "pxr/base/gf/vec3f.h"
#include "pxr/base/gf/matrix4d.h"

#include "pxr/base/tf/token.h"
#include "pxr/base/tf/type.h"

PXR_NAMESPACE_OPEN_SCOPE

class SdfAssetPath;

// -------------------------------------------------------------------------- //
// AINODEAPI                                                                  //
// -------------------------------------------------------------------------- //

/// \class UsdAiNodeAPI
///
/// API for handling prims as Arnold nodes. Provides a consistent
/// interface for getting and setting user parameters.
///
/// For any described attribute \em Fallback \em Value or \em Allowed \em Values below
/// that are text/tokens, the actual token is published and defined in \ref UsdAiTokens.
/// So to set an attribute to the value "rightHanded", use UsdAiTokens->rightHanded
/// as the value.
///
class UsdAiNodeAPI : public UsdAPISchemaBase
{
public:
    /// Compile time constant representing what kind of schema this class is.
    ///
    /// \sa UsdSchemaType
    static const UsdSchemaType schemaType = UsdSchemaType::SingleApplyAPI;

    /// Construct a UsdAiNodeAPI on UsdPrim \p prim .
    /// Equivalent to UsdAiNodeAPI::Get(prim.GetStage(), prim.GetPath())
    /// for a \em valid \p prim, but will not immediately throw an error for
    /// an invalid \p prim
    explicit UsdAiNodeAPI(const UsdPrim& prim=UsdPrim())
        : UsdAPISchemaBase(prim)
    {
    }

    /// Construct a UsdAiNodeAPI on the prim held by \p schemaObj .
    /// Should be preferred over UsdAiNodeAPI(schemaObj.GetPrim()),
    /// as it preserves SchemaBase state.
    explicit UsdAiNodeAPI(const UsdSchemaBase& schemaObj)
        : UsdAPISchemaBase(schemaObj)
    {
    }

    /// Destructor.
    USDAI_API
    virtual ~UsdAiNodeAPI();

    /// Return a vector of names of all pre-declared attributes for this schema
    /// class and all its ancestor classes.  Does not include attributes that
    /// may be authored by custom/extended methods of the schemas involved.
    USDAI_API
    static const TfTokenVector &
    GetSchemaAttributeNames(bool includeInherited=true);

    /// Return a UsdAiNodeAPI holding the prim adhering to this
    /// schema at \p path on \p stage.  If no prim exists at \p path on
    /// \p stage, or if the prim at that path does not adhere to this schema,
    /// return an invalid schema object.  This is shorthand for the following:
    ///
    /// \code
    /// UsdAiNodeAPI(stage->GetPrimAtPath(path));
    /// \endcode
    ///
    USDAI_API
    static UsdAiNodeAPI
    Get(const UsdStagePtr &stage, const SdfPath &path);


    /// Applies this <b>single-apply</b> API schema to the given \p prim.
    /// This information is stored by adding "AiNodeAPI" to the 
    /// token-valued, listOp metadata \em apiSchemas on the prim.
    /// 
    /// \return A valid UsdAiNodeAPI object is returned upon success. 
    /// An invalid (or empty) UsdAiNodeAPI object is returned upon 
    /// failure. See \ref UsdAPISchemaBase::_ApplyAPISchema() for conditions 
    /// resulting in failure. 
    /// 
    /// \sa UsdPrim::GetAppliedSchemas()
    /// \sa UsdPrim::HasAPI()
    ///
    USDAI_API
    static UsdAiNodeAPI 
    Apply(const UsdPrim &prim);

protected:
    /// Returns the type of schema this class belongs to.
    ///
    /// \sa UsdSchemaType
    USDAI_API
    virtual UsdSchemaType _GetSchemaType() const;

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
    // NODEENTRYTYPE 
    // --------------------------------------------------------------------- //
    /// The node entry type identifies the type of the node entry for each arnold node.
    /// Like, camera, shape, procedural, shader, filter, driver, etc.
    /// 
    ///
    /// \n  C++ Type: TfToken
    /// \n  Usd Type: SdfValueTypeNames->Token
    /// \n  Variability: SdfVariabilityUniform
    /// \n  Fallback Value: No Fallback
    USDAI_API
    UsdAttribute GetNodeEntryTypeAttr() const;

    /// See GetNodeEntryTypeAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    USDAI_API
    UsdAttribute CreateNodeEntryTypeAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

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

    // Create an attribute representing an Arnold user data parameter. This
    // simply places it in the "user:" namespace.
    UsdAttribute CreateUserAttribute(const TfToken &name,
                                     const SdfValueTypeName& typeName) const;

    // Return the attribute with the given name if it exists in the "user:"
    // namespace.
    UsdAttribute GetUserAttribute(const TfToken &name) const;

    // Return all attributes in the "user:" namespace.
    std::vector<UsdAttribute> GetUserAttributes() const;

    // Return all "metadata" for a given attribute.
    // Since it's not trivial to define metadata names dynamically,
    // we need to prefix the metadata with the attribute's name.
    std::vector<UsdAttribute> GetMetadataForAttribute(const UsdAttribute& attr) const;

    // Add metadata to usd attribute, the automated way.
    // We are using a pointer here, in case Solid Angle decides
    // to hide the interface in the future.
    void AddMetadataToAttribute(
        const UsdAttribute& attr,
        const AtMetaDataEntry* meta) const;

    // Adding new, dynamic metadata to an existing attribute.
    UsdAttribute AddMetadataToAttribute(
        const UsdAttribute& attr,
        const TfToken& name,
        const SdfValueTypeName& typeName,
        const VtValue& value) const;

    // Getting the metadata name from the parameter name.
    static TfToken GetMetadataNameFromAttr(const UsdAttribute& attr);

    // Returns a token from AtNodeEntry's GetType.
    static TfToken GetNodeEntryTokenFromType(int nodeEntryType);

    // Returns a node entry type from a TfToken.
    static int GetNodeEntryTypeFromToken(const TfToken& nodeEntryTypeName);

    // Returns a token from AtParamEntry's GetType.
    static TfToken GetParamTypeTokenFromType(int paramEntryType);

    // Returns a param type from a TfToken.
    static int GetParamTypeFromToken(const TfToken& paramEntryTypeName);
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
