// Copyright (c) 2018 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef HDAI_RENDER_DELEGATE_H
#define HDAI_RENDER_DELEGATE_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/renderDelegate.h>
#include <pxr/imaging/hd/renderThread.h>
#include <pxr/imaging/hd/resourceRegistry.h>

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
    AtUniverse* GetUniverse() const;

    HDAI_API
    AtNode* GetOptions() const;

    HDAI_API
    AtNode* GetFallbackShader() const;

private:
    static const TfTokenVector SUPPORTED_RPRIM_TYPES;
    static const TfTokenVector SUPPORTED_SPRIM_TYPES;
    static const TfTokenVector SUPPORTED_BPRIM_TYPES;

    static std::mutex _mutexResourceRegistry;
    static std::atomic_int _counterResourceRegistry;
    static HdResourceRegistrySharedPtr _resourceRegistry;
    HdAiRenderDelegate(const HdAiRenderDelegate&) = delete;
    HdAiRenderDelegate& operator=(const HdAiRenderDelegate&) = delete;

    AtUniverse* _universe;
    AtNode* _options;
    AtNode* _fallbackShader;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDER_DELEGATE_H
