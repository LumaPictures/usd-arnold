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
#include "pxr/imaging/hdAi/renderDelegate.h"

#include <pxr/base/tf/getenv.h>

#include <pxr/imaging/glf/glew.h>
#include <pxr/imaging/hd/bprim.h>
#include <pxr/imaging/hd/camera.h>
#include <pxr/imaging/hd/instancer.h>
#include <pxr/imaging/hd/resourceRegistry.h>
#include <pxr/imaging/hd/rprim.h>
#include <pxr/imaging/hd/tokens.h>

#include "pxr/imaging/hdAi/light.h"
#include "pxr/imaging/hdAi/material.h"
#include "pxr/imaging/hdAi/mesh.h"
#include "pxr/imaging/hdAi/nodes/nodes.h"
#include "pxr/imaging/hdAi/renderBuffer.h"
#include "pxr/imaging/hdAi/renderPass.h"

PXR_NAMESPACE_OPEN_SCOPE

const TfTokenVector HdAiRenderDelegate::SUPPORTED_RPRIM_TYPES = {
    HdPrimTypeTokens->mesh,
};

const TfTokenVector HdAiRenderDelegate::SUPPORTED_SPRIM_TYPES = {
    HdPrimTypeTokens->camera,       HdPrimTypeTokens->material,
    HdPrimTypeTokens->distantLight, HdPrimTypeTokens->sphereLight,
    HdPrimTypeTokens->diskLight,
};

const TfTokenVector HdAiRenderDelegate::SUPPORTED_BPRIM_TYPES = {
    HdPrimTypeTokens->renderBuffer,
};

std::mutex HdAiRenderDelegate::_mutexResourceRegistry;
std::atomic_int HdAiRenderDelegate::_counterResourceRegistry;
HdResourceRegistrySharedPtr HdAiRenderDelegate::_resourceRegistry;

HdAiRenderDelegate::HdAiRenderDelegate() {
    _id = SdfPath(TfToken(TfStringPrintf("/HdAiRenderDelegate_%p", this)));
    if (AiUniverseIsActive()) {
        TF_CODING_ERROR("There is already an active Arnold universe!");
    }
    AiBegin(AI_SESSION_INTERACTIVE);
    std::lock_guard<std::mutex> guard(_mutexResourceRegistry);
    if (_counterResourceRegistry.fetch_add(1) == 0) {
        _resourceRegistry.reset(new HdResourceRegistry());
    }
    AiMsgSetConsoleFlags(AI_LOG_WARNINGS | AI_LOG_ERRORS);
    hdAiInstallNodes();
    const auto arnoldPluginPath = TfGetenv("ARNOLD_PLUGIN_PATH");
    if (!arnoldPluginPath.empty()) { AiLoadPlugins(arnoldPluginPath.c_str()); }

    _universe = nullptr;

    _options = AiUniverseGetOptions(_universe);
    AiNodeSetInt(_options, "GI_diffuse_depth", 1);
    AiNodeSetInt(_options, "GI_specular_depth", 1);

    _fallbackShader = AiNode(_universe, "ambient_occlusion");
    AiNodeSetInt(_fallbackShader, "samples", 1);
}

HdAiRenderDelegate::~HdAiRenderDelegate() {
    std::lock_guard<std::mutex> guard(_mutexResourceRegistry);
    if (_counterResourceRegistry.fetch_sub(1) == 1) {
        _resourceRegistry.reset();
    }
    hdAiUninstallNodes();
    AiUniverseDestroy(_universe);
    AiEnd();
}

HdRenderParam* HdAiRenderDelegate::GetRenderParam() const { return nullptr; }

void HdAiRenderDelegate::CommitResources(HdChangeTracker* tracker) {
    TF_UNUSED(tracker);
}

const TfTokenVector& HdAiRenderDelegate::GetSupportedRprimTypes() const {
    return SUPPORTED_RPRIM_TYPES;
}

const TfTokenVector& HdAiRenderDelegate::GetSupportedSprimTypes() const {
    return SUPPORTED_SPRIM_TYPES;
}

const TfTokenVector& HdAiRenderDelegate::GetSupportedBprimTypes() const {
    return SUPPORTED_BPRIM_TYPES;
}

HdResourceRegistrySharedPtr HdAiRenderDelegate::GetResourceRegistry() const {
    return _resourceRegistry;
}

HdRenderPassSharedPtr HdAiRenderDelegate::CreateRenderPass(
    HdRenderIndex* index, const HdRprimCollection& collection) {
    return HdRenderPassSharedPtr(new HdAiRenderPass(this, index, collection));
}

HdInstancer* HdAiRenderDelegate::CreateInstancer(
    HdSceneDelegate* delegate, const SdfPath& id, const SdfPath& instancerId) {
    TF_UNUSED(delegate);
    TF_UNUSED(id);
    TF_UNUSED(instancerId);
    return nullptr;
}

void HdAiRenderDelegate::DestroyInstancer(HdInstancer* instancer) {
    delete instancer;
}

HdRprim* HdAiRenderDelegate::CreateRprim(
    const TfToken& typeId, const SdfPath& rprimId, const SdfPath& instancerId) {
    if (typeId == HdPrimTypeTokens->mesh) {
        return new HdAiMesh(this, rprimId, instancerId);
    }
    TF_CODING_ERROR("Unknown Rprim Type %s", typeId.GetText());
    return nullptr;
}

void HdAiRenderDelegate::DestroyRprim(HdRprim* rPrim) { delete rPrim; }

HdSprim* HdAiRenderDelegate::CreateSprim(
    const TfToken& typeId, const SdfPath& sprimId) {
    if (typeId == HdPrimTypeTokens->camera) { return new HdCamera(sprimId); }
    if (typeId == HdPrimTypeTokens->material) {
        return new HdAiMaterial(this, sprimId);
    }
    if (typeId == HdPrimTypeTokens->sphereLight) {
        return HdAiLight::CreatePointLight(this, sprimId);
    }
    if (typeId == HdPrimTypeTokens->distantLight) {
        return HdAiLight::CreateDistantLight(this, sprimId);
    }
    if (typeId == HdPrimTypeTokens->diskLight) {
        return HdAiLight::CreateDiskLight(this, sprimId);
    }
    TF_CODING_ERROR("Unknown Sprim Type %s", typeId.GetText());
    return nullptr;
}

HdSprim* HdAiRenderDelegate::CreateFallbackSprim(const TfToken& typeId) {
    if (typeId == HdPrimTypeTokens->camera) {
        return new HdCamera(SdfPath::EmptyPath());
    }
    if (typeId == HdPrimTypeTokens->material) {
        return new HdAiMaterial(this, SdfPath::EmptyPath());
    }
    if (typeId == HdPrimTypeTokens->sphereLight) {
        return HdAiLight::CreatePointLight(this, SdfPath::EmptyPath());
    }
    if (typeId == HdPrimTypeTokens->distantLight) {
        return HdAiLight::CreateDistantLight(this, SdfPath::EmptyPath());
    }
    if (typeId == HdPrimTypeTokens->diskLight) {
        return HdAiLight::CreateDiskLight(this, SdfPath::EmptyPath());
    }
    TF_CODING_ERROR("Unknown Sprim Type %s", typeId.GetText());
    return nullptr;
}

void HdAiRenderDelegate::DestroySprim(HdSprim* sPrim) {
    TF_UNUSED(sPrim);
    // TODO: Figure out what to do with this.
    /*delete sPrim;*/ // This crashes in HdCamera::~HdCamera. I suspect an
                      // invalid
    // prim being removed.
}

HdBprim* HdAiRenderDelegate::CreateBprim(
    const TfToken& typeId, const SdfPath& bprimId) {
    if (typeId == HdPrimTypeTokens->renderBuffer) {
        return new HdAiRenderBuffer(bprimId);
    }
    TF_CODING_ERROR("Unknown Bprim Type %s", typeId.GetText());
    return nullptr;
}

HdBprim* HdAiRenderDelegate::CreateFallbackBprim(const TfToken& typeId) {
    if (typeId == HdPrimTypeTokens->renderBuffer) {
        return new HdAiRenderBuffer(SdfPath());
    }
    TF_CODING_ERROR("Unknown Bprim Type %s", typeId.GetText());
    return nullptr;
}

void HdAiRenderDelegate::DestroyBprim(HdBprim* bPrim) { delete bPrim; }

TfToken HdAiRenderDelegate::GetMaterialBindingPurpose() const {
    return HdTokens->full;
}

AtString HdAiRenderDelegate::GetLocalNodeName(const AtString& name) const {
    return AtString(_id.AppendChild(TfToken(name.c_str())).GetText());
}

AtUniverse* HdAiRenderDelegate::GetUniverse() const { return _universe; }

AtNode* HdAiRenderDelegate::GetOptions() const { return _options; }

AtNode* HdAiRenderDelegate::GetFallbackShader() const {
    return _fallbackShader;
}

PXR_NAMESPACE_CLOSE_SCOPE
