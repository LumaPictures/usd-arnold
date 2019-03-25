// Copyright 2019 Luma Pictures
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef PXRUSDKATANA_READAOV_H
#define PXRUSDKATANA_READAOV_H

#include <pxr/pxr.h>

#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdAi/aiAOV.h>
#include <pxr/usd/usdShade/shader.h>

#include <FnLogging/FnLogging.h>

#include <usdKatana/attrMap.h>
#include <usdKatana/usdInPrivateData.h>
#include <usdKatana/utils.h>

PXR_NAMESPACE_OPEN_SCOPE

void readAiAOV(
    const UsdAiAOV& aov, const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs);

UsdShadeShader getAOVRelationshipTarget(const UsdRelationship& rel);

TfToken readAOVChildShaderPrim(
    const UsdShadeShader& shaderSchema, FnKat::GroupBuilder& paramsBuilder);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READAOV_H
