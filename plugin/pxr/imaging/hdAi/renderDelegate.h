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
#ifndef HDAI_RENDER_DELEGATE_H
#define HDAI_RENDER_DELEGATE_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/renderDelegate.h>
#include <pxr/imaging/hd/renderThread.h>
#include <pxr/imaging/hd/resourceRegistry.h>

#include "pxr/imaging/hdAi/renderParam.h"

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderDelegate final : public HdRenderDelegate {
public:
    HDAI_API
    HdAiRenderDelegate();
    HDAI_API
    ~HdAiRenderDelegate() override;
    HDAI_API
    HdRenderParam* GetRenderParam() const override;
    HDAI_API
    const TfTokenVector& GetSupportedRprimTypes() const override;
    HDAI_API
    const TfTokenVector& GetSupportedSprimTypes() const override;
    HDAI_API
    const TfTokenVector& GetSupportedBprimTypes() const override;
    HDAI_API
    void SetRenderSetting(const TfToken& key, const VtValue& value) override;
    HDAI_API
    VtValue GetRenderSetting(const TfToken& key) const override;
    HDAI_API
    HdRenderSettingDescriptorList GetRenderSettingDescriptors() const override;
    HDAI_API
    HdResourceRegistrySharedPtr GetResourceRegistry() const override;
    HDAI_API
    HdRenderPassSharedPtr CreateRenderPass(
        HdRenderIndex* index, HdRprimCollection const& collection) override;
    HDAI_API
    HdInstancer* CreateInstancer(
        HdSceneDelegate* delegate, SdfPath const& id,
        SdfPath const& instancerId) override;
    HDAI_API
    void DestroyInstancer(HdInstancer* instancer) override;
    HDAI_API
    HdRprim* CreateRprim(
        TfToken const& typeId, SdfPath const& rprimId,
        SdfPath const& instancerId) override;
    HDAI_API
    void DestroyRprim(HdRprim* rPrim) override;
    HDAI_API
    HdSprim* CreateSprim(
        TfToken const& typeId, SdfPath const& sprimId) override;
    HDAI_API
    HdSprim* CreateFallbackSprim(TfToken const& typeId) override;
    HDAI_API
    void DestroySprim(HdSprim* sPrim) override;
    HDAI_API
    HdBprim* CreateBprim(
        TfToken const& typeId, SdfPath const& bprimId) override;
    HDAI_API
    HdBprim* CreateFallbackBprim(TfToken const& typeId) override;
    HDAI_API
    void DestroyBprim(HdBprim* bPrim) override;
    HDAI_API
    void CommitResources(HdChangeTracker* tracker) override;
    HDAI_API
    TfToken GetMaterialBindingPurpose() const override;

    HDAI_API
    AtString GetLocalNodeName(const AtString& name) const;

    HDAI_API
    AtUniverse* GetUniverse() const;

    HDAI_API
    AtNode* GetOptions() const;

    HDAI_API
    AtNode* GetFallbackShader() const;

private:
    static std::mutex _mutexResourceRegistry;
    static std::atomic_int _counterResourceRegistry;
    static HdResourceRegistrySharedPtr _resourceRegistry;
    HdAiRenderDelegate(const HdAiRenderDelegate&) = delete;
    HdAiRenderDelegate& operator=(const HdAiRenderDelegate&) = delete;

    std::unique_ptr<HdAiRenderParam> _renderParam;
    SdfPath _id;
    AtUniverse* _universe;
    AtNode* _options;
    AtNode* _fallbackShader;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDER_DELEGATE_H
