#ifndef PXRUSDKATANA_ARNOLDHELPERS_H
#define PXRUSDKATANA_ARNOLDHELPERS_H

#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/attribute.h>

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
        const UsdPrim& prim,
        FnKat::GroupBuilder& argsBuilder,
        const double time);

// Given a prim, return a new GroupAttribute to populate the `arnoldStatements`
// attribute group in Katana.
FnKat::Attribute getArnoldStatementsGroup(const UsdPrim& prim);

// Based on PxrUsdKatanaAttrMap::toInterface
void updateOrCreateAttr(
        FnKat::GeolibCookInterface& interface,
        const std::string& attrName,
        const FnKat::Attribute& attr);

template <typename S>
struct AttributeDefinition {
    typedef UsdAttribute (S::*queryFn_t)() const;
    queryFn_t queryFn;
    const char* paramName;

    inline AttributeDefinition(
        queryFn_t _queryFn,
        const char* _paramName) : queryFn(_queryFn), paramName(_paramName) {}
};

template <typename T, typename S>
struct OptionalAttributeDefinition : public AttributeDefinition<S> {
    T defaultValue;

    inline OptionalAttributeDefinition(
        decltype(AttributeDefinition<S>::queryFn) _queryFn,
        const char* _paramName,
        const T& _defaultValue) : AttributeDefinition<S>(_queryFn, _paramName), defaultValue(_defaultValue) {}
};

template <typename T> inline
FnKat::Attribute createAttribute(const T& v) {
    return FnKat::IntAttribute(static_cast<int>(v));
}

template <> inline
FnKat::Attribute createAttribute(const bool& v) {
    return FnKat::IntAttribute(v ? 1 : 0);
}

template <> inline
FnKat::Attribute createAttribute(const float& v) {
    return FnKat::FloatAttribute(v);
}

template <> inline
FnKat::Attribute createAttribute(const TfToken& v) {
    return FnKat::StringAttribute(v.GetString());
}

template <typename T, typename S> inline
bool handleAttributes(const std::vector<OptionalAttributeDefinition<T, S>>& attributes,
                       const S& api,
                       FnKat::GroupBuilder& builder) {
    auto attributeSet = false;
    for (const auto& each : attributes) {
        const auto attr = ((api).*(each.queryFn))();
        if (!attr.IsValid()) { continue; }
        T v = each.defaultValue;
        // TODO: Check if we need to filter the defaultValues.
        // I think because of how Katana behaves we have to set these up,
        // even if they are the default value, because the USD API handles
        // the concept of an attribute not being set. Which doesn't work in Arnold.
        if (attr.Get(&v) && v != each.defaultValue) {
            builder.set(each.paramName, createAttribute(v));
            attributeSet = true;
        }
    }
    return attributeSet;
}

template <typename T, typename S> inline
bool handleAttributes(const std::vector<AttributeDefinition<S>>& attributes,
                      const S& api,
                      FnKat::GroupBuilder& builder) {
    auto attributeSet = false;
    for (const auto& each : attributes) {
        const auto attr = ((api).*(each.queryFn))();
        if (!attr.IsValid()) { continue; }
        T v;
        if (attr.Get(&v)) {
            builder.set(each.paramName, createAttribute(v));
            attributeSet = true;
        }
    }
    return attributeSet;
}

PXR_NAMESPACE_CLOSE_SCOPE

#endif
