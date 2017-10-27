#ifndef PXRUSDKATANA_READAOV_H
#define PXRUSDKATANA_READAOV_H

#include <pxr/pxr.h>

#include <pxr/usd/usd/prim.h>
#include <FnAttribute/FnAttribute.h>

#include <pxr/usd/usdAi/aiAOV.h>

PXR_NAMESPACE_OPEN_SCOPE

// Given a prim, return a new GroupAttribute to setup an AOV
FnKat::Attribute getAOV(const UsdAiAOV& aov);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READAOV_H
