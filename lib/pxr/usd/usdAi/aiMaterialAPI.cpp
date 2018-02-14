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
        TfType::Bases< UsdSchemaBase > >();
    
}

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


/* static */
UsdAiMaterialAPI
UsdAiMaterialAPI::Apply(const UsdStagePtr &stage, const SdfPath &path)
{
    // Ensure we have a valid stage, path and prim
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return UsdAiMaterialAPI();
    }

    if (path == SdfPath::AbsoluteRootPath()) {
        TF_CODING_ERROR("Cannot apply an api schema on the pseudoroot");
        return UsdAiMaterialAPI();
    }

    auto prim = stage->GetPrimAtPath(path);
    if (!prim) {
        TF_CODING_ERROR("Prim at <%s> does not exist.", path.GetText());
        return UsdAiMaterialAPI();
    }

    TfToken apiName("AiMaterialAPI");  

    // Get the current listop at the edit target
    UsdEditTarget editTarget = stage->GetEditTarget();
    SdfPrimSpecHandle primSpec = editTarget.GetPrimSpecForScenePath(path);
    SdfTokenListOp listOp = primSpec->GetInfo(UsdTokens->apiSchemas)
                                    .UncheckedGet<SdfTokenListOp>();

    // Append our name to the prepend list, if it doesnt exist locally
    TfTokenVector prepends = listOp.GetPrependedItems();
    if (std::find(prepends.begin(), prepends.end(), apiName) != prepends.end()) { 
        return UsdAiMaterialAPI();
    }

    SdfTokenListOp prependListOp;
    prepends.push_back(apiName);
    prependListOp.SetPrependedItems(prepends);
    auto result = listOp.ApplyOperations(prependListOp);
    if (!result) {
        TF_CODING_ERROR("Failed to prepend api name to current listop.");
        return UsdAiMaterialAPI();
    }

    // Set the listop at the current edit target and return the API prim
    primSpec->SetInfo(UsdTokens->apiSchemas, VtValue(*result));
    return UsdAiMaterialAPI(prim);
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

/*static*/
const TfTokenVector&
UsdAiMaterialAPI::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames;
    static TfTokenVector allNames =
        UsdSchemaBase::GetSchemaAttributeNames(true);

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
