#ifndef PXRUSDKATANA_READAOV_H
#define PXRUSDKATANA_READAOV_H

#include <pxr/pxr.h>

#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usdAi/aiAOV.h>

#include <FnLogging/FnLogging.h>

#include <usdKatana/attrMap.h>
#include <usdKatana/usdInPrivateData.h>
#include <usdKatana/utils.h>


PXR_NAMESPACE_OPEN_SCOPE


void readAiAOV(
    const UsdAiAOV& aov,
    const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs);

UsdShadeShader
getAOVRelationshipTarget(
    const UsdRelationship& rel);

TfToken
readAOVChildShaderPrim(
    const UsdShadeShader& shaderSchema,
    FnKat::GroupBuilder& paramsBuilder);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READAOV_H
