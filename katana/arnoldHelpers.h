#ifndef PXRUSDKATANA_ARNOLDHELPERS_H
#define PXRUSDKATANA_ARNOLDHELPERS_H

#include <pxr/usd/usd/prim.h>

#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
#include <FnGeolib/op/FnGeolibCookInterface.h>

PXR_NAMESPACE_OPEN_SCOPE

// Utility functions for handling Arnold nodes stored in USD files.

class SdfValueTypeName;

// Given an SDF value type, return a (possibly empty) type hint string for KtoA.
std::string getArnoldAttrTypeHint(const SdfValueTypeName& scalarType);

// Given a prim, return a new GroupAttribute to apply as its `arnoldStatements`
// attribute in Katana.
FnKat::Attribute getArnoldStatementsGroup(const UsdPrim& prim);

// Based on PxrUsdKatanaAttrMap::toInterface
void updateOrCreateAttr(
        FnKat::GeolibCookInterface& interface,
        const std::string& attrName,
        const FnKat::Attribute& attr);

PXR_NAMESPACE_CLOSE_SCOPE

#endif
