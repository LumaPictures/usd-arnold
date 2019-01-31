// Copyright (c) 2019 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef PXRUSDKATANA_ARNOLDHELPERS_H
#define PXRUSDKATANA_ARNOLDHELPERS_H

#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/prim.h>

#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
#include <FnGeolib/op/FnGeolibCookInterface.h>

PXR_NAMESPACE_OPEN_SCOPE

// Utility functions for handling Arnold nodes stored in USD files.

class SdfValueTypeName;

// Given an SDF value type, return a (possibly empty) type hint string for KtoA.
std::string getArnoldAttrTypeHint(const SdfValueTypeName& scalarType);

// Given a prim representing an Arnold procedural and a GroupBuilder, populate
// the builder by converting prim attributes in the "user:" namespace to their
// canonical Katana attribute types. Returns the number of converted attributes.
size_t applyProceduralArgsAttrs(
    const UsdPrim& prim, FnKat::GroupBuilder& argsBuilder, const double time);

// Helper to populate a GroupBuilder with most of the argument defaults needed
// to execute an ArnoldOpenVDBVolume op. This uses the UsdAiVolumeAPI to query
// the given prim
void getArnoldVDBVolumeOpArgs(
    const UsdPrim& prim, FnKat::GroupBuilder& argsBuilder);

// Given a prim, return a new GroupAttribute to populate the `arnoldStatements`
// attribute group in Katana.
FnKat::Attribute getArnoldStatementsGroup(const UsdPrim& prim);

// Based on PxrUsdKatanaAttrMap::toInterface
void updateOrCreateAttr(
    FnKat::GeolibCookInterface& interface, const std::string& attrName,
    const FnKat::Attribute& attr);

template <typename S>
struct AttributeDefinition {
    typedef UsdAttribute (S::*queryFn_t)() const;
    queryFn_t queryFn;
    const char* paramName;

    inline AttributeDefinition(queryFn_t _queryFn, const char* _paramName)
        : queryFn(_queryFn), paramName(_paramName) {}
};

template <typename T, typename S>
struct OptionalAttributeDefinition : public AttributeDefinition<S> {
    T defaultValue;

    inline OptionalAttributeDefinition(
        decltype(AttributeDefinition<S>::queryFn) _queryFn,
        const char* _paramName, const T& _defaultValue)
        : AttributeDefinition<S>(_queryFn, _paramName),
          defaultValue(_defaultValue) {}
};

template <typename T>
inline FnKat::Attribute createAttribute(const T& v) {
    return FnKat::IntAttribute(static_cast<int>(v));
}

template <>
inline FnKat::Attribute createAttribute(const bool& v) {
    return FnKat::IntAttribute(v ? 1 : 0);
}

template <>
inline FnKat::Attribute createAttribute(const float& v) {
    return FnKat::FloatAttribute(v);
}

template <>
inline FnKat::Attribute createAttribute(const TfToken& v) {
    return FnKat::StringAttribute(v.GetString());
}

template <typename T, typename S>
inline void handleAttributes(
    const std::vector<OptionalAttributeDefinition<T, S>>& attributes,
    const S& api, FnKat::GroupBuilder& builder, bool applyDefaults = false) {
    for (const auto& each : attributes) {
        const auto attr = ((api).*(each.queryFn))();
        T value = each.defaultValue;
        // TODO: Check if we need to filter the defaultValues.
        // I think because of how Katana behaves we have to set these up,
        // even if they are the default value, because the USD API handles
        // the concept of an attribute not being set. Which doesn't work in
        // Arnold.
        const bool success = attr.IsValid() && attr.template Get<T>(&value);
        if (applyDefaults || (success && value != each.defaultValue)) {
            builder.set(each.paramName, createAttribute(value));
        }
    }
}

template <typename T, typename S>
inline void handleAttributes(
    const std::vector<AttributeDefinition<S>>& attributes, const S& api,
    FnKat::GroupBuilder& builder) {
    for (const auto& each : attributes) {
        if (const auto attr = ((api).*(each.queryFn))()) {
            T value;
            if (attr.template Get<T>(&value)) {
                builder.set(each.paramName, createAttribute(value));
            }
        }
    }
}

PXR_NAMESPACE_CLOSE_SCOPE

#endif
