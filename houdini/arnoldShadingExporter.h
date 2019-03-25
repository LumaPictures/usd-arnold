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
#ifndef __USDAI_ARNOLDSHADINGEXPORTER_H__
#define __USDAI_ARNOLDSHADINGEXPORTER_H__

#include <pxr/pxr.h>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/sdf/path.h>

#include <gusd/shadingModeRegistry.h>

#include <OP/OP_Node.h>

#include <string>

PXR_NAMESPACE_OPEN_SCOPE

void exportArnoldShaders(
    OP_Node* opNode,
    const UsdStagePtr& stage,
    const SdfPath& looksPath,
    const GusdShadingModeRegistry::HouMaterialMap& houMaterialMap,
    const std::string& shaderOutDir);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // __USDAI_ARNOLDSHADINGEXPORTER_H__
