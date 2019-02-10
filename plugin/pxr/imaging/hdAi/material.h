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
#ifndef HDAI_MATERIAL_H
#define HDAI_MATERIAL_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/material.h>

#include "pxr/imaging/hdAi/renderDelegate.h"

#include <ai.h>

#include <unordered_map>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiMaterial : public HdMaterial {
public:
    HDAI_API
    HdAiMaterial(HdAiRenderDelegate* delegate, const SdfPath& id);

    ~HdAiMaterial() override;

    HDAI_API
    void Sync(
        HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
        HdDirtyBits* dirtyBits) override;

    HDAI_API
    HdDirtyBits GetInitialDirtyBitsMask() const override;
    HDAI_API
    void Reload() override;
    HDAI_API
    AtNode* GetSurfaceShader() const;
    HDAI_API
    AtNode* GetDisplacementShader() const;

protected:
    HDAI_API
    AtNode* ReadMaterialNetwork(const HdMaterialNetwork& network);

    HDAI_API
    AtNode* ReadMaterial(const HdMaterialNode& node);

    HDAI_API
    AtNode* FindMaterial(const SdfPath& id) const;

    HDAI_API
    AtString GetLocalNodeName(const SdfPath& path) const;

    std::unordered_map<AtString, AtNode*, AtStringHash> _nodes;
    HdAiRenderDelegate* _delegate;
    AtNode* _surface = nullptr;
    AtNode* _displacement = nullptr;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_MATERIAL_H
