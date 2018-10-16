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
#include "pxr/usd/usdAi/aiNodeAPI.h"
#include "pxr/usd/usd/schemaRegistry.h"
#include "pxr/usd/usd/typed.h"
#include "pxr/usd/usd/tokens.h"

#include "pxr/usd/sdf/types.h"
#include "pxr/usd/sdf/assetPath.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the schema with the TfType system.
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<UsdAiNodeAPI,
        TfType::Bases< UsdAPISchemaBase > >();
    
}

TF_DEFINE_PRIVATE_TOKENS(
    _schemaTokens,
    (AiNodeAPI)
);

/* virtual */
UsdAiNodeAPI::~UsdAiNodeAPI()
{
}

/* static */
UsdAiNodeAPI
UsdAiNodeAPI::Get(const UsdStagePtr &stage, const SdfPath &path)
{
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return UsdAiNodeAPI();
    }
    return UsdAiNodeAPI(stage->GetPrimAtPath(path));
}


/* virtual */
UsdSchemaType UsdAiNodeAPI::_GetSchemaType() const {
    return UsdAiNodeAPI::schemaType;
}

/* static */
UsdAiNodeAPI
UsdAiNodeAPI::Apply(const UsdPrim &prim)
{
    return UsdAPISchemaBase::_ApplyAPISchema<UsdAiNodeAPI>(
            prim, _schemaTokens->AiNodeAPI);
}

/* static */
const TfType &
UsdAiNodeAPI::_GetStaticTfType()
{
    static TfType tfType = TfType::Find<UsdAiNodeAPI>();
    return tfType;
}

/* static */
bool 
UsdAiNodeAPI::_IsTypedSchema()
{
    static bool isTyped = _GetStaticTfType().IsA<UsdTyped>();
    return isTyped;
}

/* virtual */
const TfType &
UsdAiNodeAPI::_GetTfType() const
{
    return _GetStaticTfType();
}

UsdAttribute
UsdAiNodeAPI::GetNodeEntryTypeAttr() const
{
    return GetPrim().GetAttribute(UsdAiTokens->infoNode_entry_type);
}

UsdAttribute
UsdAiNodeAPI::CreateNodeEntryTypeAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(UsdAiTokens->infoNode_entry_type,
                       SdfValueTypeNames->Token,
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
UsdAiNodeAPI::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames = {
        UsdAiTokens->infoNode_entry_type,
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

#include "pxr/usd/usdAi/aiShaderExport.h"
#include "pxr/usd/usdAi/tokens.h"
#include <ai.h>
#include <initializer_list>

PXR_NAMESPACE_OPEN_SCOPE

UsdAttribute
UsdAiNodeAPI::CreateUserAttribute(const TfToken &name,
                                  const SdfValueTypeName& typeName) const
{
    return GetPrim().CreateAttribute(
        TfToken(UsdAiTokens->userPrefix.GetString() + name.GetString()),
        typeName);
}

UsdAttribute
UsdAiNodeAPI::GetUserAttribute(const TfToken &name) const
{
    return GetPrim().GetAttribute(
        TfToken(UsdAiTokens->userPrefix.GetString() + name.GetString()));
}

std::vector<UsdAttribute>
UsdAiNodeAPI::GetUserAttributes() const
{
    std::vector<UsdAttribute> result;

    for (const auto& it: GetPrim().GetAttributes()) {
        if (TfStringStartsWith(it.GetName().GetString(),
                               UsdAiTokens->userPrefix)) {
            result.push_back(it);
        }
    }
    return result;
}

void
UsdAiNodeAPI::AddMetadataToAttribute(
    const UsdAttribute& attr,
    const AtMetaDataEntry* meta) const {
    if (meta == nullptr) { return; }
    const auto* meta_conversion = AiShaderExport::get_default_value_conversion(meta->type);
    if (meta_conversion == nullptr) { return; }
    auto value = meta_conversion->f == nullptr ? VtValue() : meta_conversion->f(meta->value, nullptr);
    AddMetadataToAttribute(attr, TfToken(meta->name.c_str()), meta_conversion->type, value).
        SetMetadata(UsdAiTokens->paramType, GetParamTypeTokenFromType(meta->type));
}

UsdAttribute
UsdAiNodeAPI::AddMetadataToAttribute(
    const UsdAttribute& attr,
    const TfToken& name,
    const SdfValueTypeName& typeName,
    const VtValue& value) const {
    auto metaName = name.GetString();
    std::replace(metaName.begin(), metaName.end(), '.', ':');
    metaName = attr.GetName().GetString() + ":" + metaName;
    auto metaAttr = GetPrim().CreateAttribute(TfToken(metaName), typeName, false, SdfVariabilityUniform);
    metaAttr.Set(value);
    return metaAttr;
}

TfToken
UsdAiNodeAPI::GetMetadataNameFromAttr(const UsdAttribute& attr) {
    auto name = attr.GetName().GetString();
    const auto colorPos = name.find(':');
    if (colorPos == std::string::npos) { return TfToken(name); }
    name = name.substr(colorPos + 1);
    std::replace(name.begin(), name.end(), ':', '.');
    return TfToken(name);
}

std::vector<UsdAttribute>
UsdAiNodeAPI::GetMetadataForAttribute(const UsdAttribute& attr) const {
    const auto attrName = attr.GetName();
    std::vector<UsdAttribute> result;
    for (const auto& it: GetPrim().GetAttributes()) {
        if (TfStringStartsWith(it.GetName(), TfToken(attrName.GetString() + ":"))) {
            result.push_back(it);
        }
    }
    return result;
}

namespace {
// C here is a bit ugly, but well... No typeclasses or traits, yet!
// TODO: improve this, so type deductions work better!
template <typename C, typename I, typename V> std::vector<std::tuple<I, V>>
_getSortedTupleVector(
    const C& list,
    std::function<bool(const I&, const I&)> sf = [](const I& a, const I& b) -> bool { return a < b; }) {
    std::vector<std::tuple<I, V>> ret(list);
    std::sort(ret.begin(), ret.end(),
              [&sf](const std::tuple<I, V>& a, const std::tuple<I, V>& b) -> bool {
                  return sf(std::get<0>(a), std::get<0>(b));
              });
    return ret;
}

// No function alias with using
template <typename... Args> auto
_m(Args&& ... args) -> decltype(std::make_tuple(std::forward<Args>(args)...)) {
    return std::make_tuple(std::forward<Args>(args)...);
}

static const TfToken _undefinedToken("undefined");

template <typename I, typename V> inline
V _getValueFromSortedTupleVector(
    const std::vector<std::tuple<I, V>>& v,
    const I& i, const V& dv,
    std::function<bool(const I&, const I&)> sf = [](const I& a, const I& b) -> bool { return a < b; }) {
    const auto it = std::lower_bound(
        v.begin(), v.end(),
        _m(i, dv), [&sf](const std::tuple<I, V>& a, const std::tuple<I, V>& b) -> bool {
            return sf(std::get<0>(a), std::get<0>(b));
        });

    if (it == v.end() || std::get<0>(*it) != i) {
        return dv;
    }
    return std::get<1>(*it);
};

const auto _nodeEntryTypeToToken =
    _getSortedTupleVector<std::initializer_list<std::tuple<int, TfToken>>, int, TfToken>(
        {
            _m(AI_NODE_UNDEFINED, _undefinedToken),
            _m(AI_NODE_OPTIONS, TfToken("options")),
            _m(AI_NODE_CAMERA, TfToken("camera")),
            _m(AI_NODE_LIGHT, TfToken("light")),
            _m(AI_NODE_SHAPE, TfToken("shape")),
            _m(AI_NODE_SHADER, TfToken("shader")),
            _m(AI_NODE_OVERRIDE, TfToken("override")),
            _m(AI_NODE_DRIVER, TfToken("driver")),
            _m(AI_NODE_FILTER, TfToken("filter")),
            _m(AI_NODE_COLOR_MANAGER, TfToken("color_manager")),
            _m(AI_NODE_SHAPE_PROCEDURAL, TfToken("procedural")),
            _m(AI_NODE_SHAPE_VOLUME, TfToken("volume")),
            _m(AI_NODE_SHAPE_IMPLICIT, TfToken("implicit")),
            _m(AI_NODE_ALL, TfToken("all")),
        });

const auto _paramTypeToToken =
    _getSortedTupleVector<std::initializer_list<std::tuple<int, TfToken>>, int, TfToken>(
        {
            _m(AI_TYPE_BYTE, TfToken("byte")),
            _m(AI_TYPE_INT, TfToken("int")),
            _m(AI_TYPE_UINT, TfToken("uint")),
            _m(AI_TYPE_BOOLEAN, TfToken("boolean")),
            _m(AI_TYPE_FLOAT, TfToken("float")),
            _m(AI_TYPE_RGB, TfToken("rgb")),
            _m(AI_TYPE_RGBA, TfToken("rgba")),
            _m(AI_TYPE_VECTOR, TfToken("vector")),
            _m(AI_TYPE_VECTOR2, TfToken("vector2")),
            _m(AI_TYPE_STRING, TfToken("string")),
            _m(AI_TYPE_POINTER, TfToken("pointer")),
            _m(AI_TYPE_NODE, TfToken("node")),
            _m(AI_TYPE_ARRAY, TfToken("array")),
            _m(AI_TYPE_MATRIX, TfToken("matrix")),
            _m(AI_TYPE_ENUM, TfToken("enum")),
            _m(AI_TYPE_CLOSURE, TfToken("closure")),
            _m(AI_TYPE_USHORT, TfToken("ushort")),
            _m(AI_TYPE_HALF, TfToken("half")),
            _m(AI_TYPE_UNDEFINED, TfToken("undefined")),
            _m(AI_TYPE_NONE, TfToken("none"))
        });

template <typename A, typename B> inline std::vector<std::tuple<B, A>>
_flipTupleVector(const std::vector<std::tuple<A, B>>& v) {
    std::vector<std::tuple<B, A>> ret;
    ret.reserve(v.size());
    for (const auto& it: v) {
        ret.emplace_back(std::get<1>(it), std::get<0>(it));
    }
    return ret;
}

auto _tokenSort = [](const TfToken& a, const TfToken& b) -> bool {
    return TfToken::LTTokenFunctor()(a, b);
};

// TODO: sort Tokens.
const auto _tokenToNodeEntryType = _getSortedTupleVector<std::vector<std::tuple<TfToken, int>>, TfToken, int>(_flipTupleVector(_nodeEntryTypeToToken), _tokenSort);
const auto _tokenToParamType = _getSortedTupleVector<std::vector<std::tuple<TfToken, int>>, TfToken, int>(_flipTupleVector(_paramTypeToToken), _tokenSort);
}

TfToken
UsdAiNodeAPI::GetNodeEntryTokenFromType(int nodeEntryType) {
    return _getValueFromSortedTupleVector(_nodeEntryTypeToToken, nodeEntryType, _undefinedToken);
}

int
UsdAiNodeAPI::GetNodeEntryTypeFromToken(const TfToken& nodeEntryTypeName) {
    return _getValueFromSortedTupleVector<TfToken, int>(_tokenToNodeEntryType, nodeEntryTypeName, AI_NODE_UNDEFINED, _tokenSort);
}

TfToken
UsdAiNodeAPI::GetParamTypeTokenFromType(int paramEntryType) {
    return _getValueFromSortedTupleVector(_paramTypeToToken, paramEntryType, _undefinedToken);
}

int
UsdAiNodeAPI::GetParamTypeFromToken(const TfToken& paramEntryTypeName) {
    return _getValueFromSortedTupleVector<TfToken, int>(_tokenToParamType, paramEntryTypeName, AI_TYPE_UNDEFINED, _tokenSort);
}

PXR_NAMESPACE_CLOSE_SCOPE
