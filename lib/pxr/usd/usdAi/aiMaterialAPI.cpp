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
#include "pxr/usd/usdAi/aiMaterialAPI.h"
#include "pxr/usd/usd/schemaRegistry.h"
#include "pxr/usd/usd/typed.h"
#include "pxr/usd/usd/tokens.h"

#include "pxr/usd/sdf/types.h"
#include "pxr/usd/sdf/assetPath.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the schema with the TfType system.
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<UsdAiMaterialAPI,
        TfType::Bases< UsdAPISchemaBase > >();
    
}

TF_DEFINE_PRIVATE_TOKENS(
    _schemaTokens,
    (AiMaterialAPI)
);

/* virtual */
UsdAiMaterialAPI::~UsdAiMaterialAPI()
{
}

/* static */
UsdAiMaterialAPI
UsdAiMaterialAPI::Get(const UsdStagePtr &stage, const SdfPath &path)
{
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return UsdAiMaterialAPI();
    }
    return UsdAiMaterialAPI(stage->GetPrimAtPath(path));
}


/* virtual */
UsdSchemaType UsdAiMaterialAPI::_GetSchemaType() const {
    return UsdAiMaterialAPI::schemaType;
}

/* static */
UsdAiMaterialAPI
UsdAiMaterialAPI::Apply(const UsdPrim &prim)
{
    return UsdAPISchemaBase::_ApplyAPISchema<UsdAiMaterialAPI>(
            prim, _schemaTokens->AiMaterialAPI);
}

/* static */
const TfType &
UsdAiMaterialAPI::_GetStaticTfType()
{
    static TfType tfType = TfType::Find<UsdAiMaterialAPI>();
    return tfType;
}

/* static */
bool 
UsdAiMaterialAPI::_IsTypedSchema()
{
    static bool isTyped = _GetStaticTfType().IsA<UsdTyped>();
    return isTyped;
}

/* virtual */
const TfType &
UsdAiMaterialAPI::_GetTfType() const
{
    return _GetStaticTfType();
}

UsdRelationship
UsdAiMaterialAPI::GetSurfaceRel() const
{
    return GetPrim().GetRelationship(UsdAiTokens->aiSurface);
}

UsdRelationship
UsdAiMaterialAPI::CreateSurfaceRel() const
{
    return GetPrim().CreateRelationship(UsdAiTokens->aiSurface,
                       /* custom = */ false);
}

UsdRelationship
UsdAiMaterialAPI::GetDisplacementRel() const
{
    return GetPrim().GetRelationship(UsdAiTokens->aiDisplacement);
}

UsdRelationship
UsdAiMaterialAPI::CreateDisplacementRel() const
{
    return GetPrim().CreateRelationship(UsdAiTokens->aiDisplacement,
                       /* custom = */ false);
}

UsdRelationship
UsdAiMaterialAPI::GetVolumeRel() const
{
    return GetPrim().GetRelationship(UsdAiTokens->aiVolume);
}

UsdRelationship
UsdAiMaterialAPI::CreateVolumeRel() const
{
    return GetPrim().CreateRelationship(UsdAiTokens->aiVolume,
                       /* custom = */ false);
}

/*static*/
const TfTokenVector&
UsdAiMaterialAPI::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames;
    static TfTokenVector allNames =
        UsdAPISchemaBase::GetSchemaAttributeNames(true);

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
