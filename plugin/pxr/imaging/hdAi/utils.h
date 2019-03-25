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
#ifndef HDAI_UTILS_H
#define HDAI_UTILS_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/matrix4f.h>

#include <pxr/base/vt/value.h>

#include <pxr/imaging/hd/sceneDelegate.h>

#include <ai.h>

#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

HDAI_API
AtMatrix HdAiConvertMatrix(const GfMatrix4d& in);
HDAI_API
AtMatrix HdAiConvertMatrix(const GfMatrix4f& in);
HDAI_API
GfMatrix4f HdAiConvertMatrix(const AtMatrix& in);
HDAI_API
void HdAiSetTransform(
    AtNode* node, HdSceneDelegate* delegate, const SdfPath& id);
HDAI_API
void HdAiSetTransform(
    std::vector<AtNode*>& nodes, HdSceneDelegate* delegate, const SdfPath& id);
HDAI_API
void HdAiSetParameter(
    AtNode* node, const AtParamEntry* pentry, const VtValue& value);
HDAI_API
void HdAiSetConstantPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc);
HDAI_API
void HdAiSetUniformPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc);
HDAI_API
void HdAiSetVertexPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc);
HDAI_API
void HdAiSetFaceVaryingPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_UTILS_H
