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
#ifndef HDAI_LIGHT_H
#define HDAI_LIGHT_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/light.h>

#include "pxr/imaging/hdAi/renderDelegate.h"

#include <functional>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiLight : public HdLight {
public:
    HDAI_API
    static HdAiLight* CreatePointLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    HDAI_API
    static HdAiLight* CreateDistantLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    HDAI_API
    static HdAiLight* CreateDiskLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    HDAI_API
    static HdAiLight* CreateRectLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    HDAI_API
    static HdAiLight* CreateCylinderLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    HDAI_API
    static HdAiLight* CreateDomeLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    HDAI_API
    void Sync(
        HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
        HdDirtyBits* dirtyBits) override;

    HdDirtyBits GetInitialDirtyBitsMask() const override;

protected:
    using SyncParams = std::function<void(
        AtNode*, const AtNodeEntry*, const SdfPath&, HdSceneDelegate*)>;

    HDAI_API
    HdAiLight(
        HdAiRenderDelegate* delegate, const SdfPath& id,
        const AtString& arnoldType, const SyncParams& sync,
        bool supportsTexture = false);

    HDAI_API
    ~HdAiLight() override;

    HDAI_API
    void SetupTexture(const VtValue& value);

    SyncParams _syncParams;
    HdAiRenderDelegate* _delegate;
    AtNode* _light;
    AtNode* _texture = nullptr;
    bool _supportsTexture = false;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_LIGHT_H
