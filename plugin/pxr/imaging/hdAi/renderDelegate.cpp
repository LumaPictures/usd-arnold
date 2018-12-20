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

#include <unordered_map>
#include <unordered_set>

PXR_NAMESPACE_OPEN_SCOPE

namespace {
namespace Str {
const AtString GI_diffuse_depth("GI_diffuse_depth");
const AtString GI_specular_depth("GI_specular_depth");
} // namespace Str

std::unordered_set<AtString, AtStringHash> _ignoredParameters{
    AtString("xres"),
    AtString("yres"),
    AtString("region_min_x"),
    AtString("region_min_y"),
    AtString("region_max_x"),
    AtString("region_max_y"),
    AtString("pixel_aspect_ratio"),
    AtString("reference_time"),
    AtString("frame"),
    AtString("fps"),
    AtString("enable_progressive_render"),
    AtString("enable_progressive_pattern")};

std::unordered_map<AtString, TfToken, AtStringHash> _parameterNameMap{
    {AtString("GI_diffuse_depth"), TfToken("Diffuse Depth")},
    {AtString("GI_specular_depth"), TfToken("Specular Depth")},
    {AtString("GI_transmission_depth"), TfToken("Transmission Depth")},
    {AtString("GI_volume_depth"), TfToken("Volume Depth")},
    {AtString("GI_total_depth"), TfToken("Total Depth")},
    {AtString("GI_diffuse_samples"), TfToken("Diffuse Samples")},
    {AtString("GI_specular_samples"), TfToken("Specular Samples")},
    {AtString("GI_transmission_samples"), TfToken("Transmission Samples")},
    {AtString("GI_sss_samples"), TfToken("SSS Samples")},
    {AtString("GI_volume_samples"), TfToken("Volume Samples")},
};

TfToken _GetParameterLabel(const AtString& name) {
    const auto& it = _parameterNameMap.find(name);
    return it == _parameterNameMap.end() ? TfToken(name.c_str()) : it->second;
}

} // namespace

const TfTokenVector HdAiRenderDelegate::SUPPORTED_RPRIM_TYPES = {
    HdPrimTypeTokens->mesh,
};

const TfTokenVector HdAiRenderDelegate::SUPPORTED_SPRIM_TYPES = {
    HdPrimTypeTokens->camera,        HdPrimTypeTokens->material,
    HdPrimTypeTokens->distantLight,  HdPrimTypeTokens->sphereLight,
    HdPrimTypeTokens->diskLight,     HdPrimTypeTokens->rectLight,
    HdPrimTypeTokens->cylinderLight, HdPrimTypeTokens->domeLight};

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
    AiNodeSetInt(_options, Str::GI_diffuse_depth, 1);
    AiNodeSetInt(_options, Str::GI_specular_depth, 1);

    _fallbackShader = AiNode(_universe, "ambient_occlusion");
    AiNodeSetInt(_fallbackShader, "samples", 1);
    _renderParam.reset(new HdAiRenderParam());
}

HdAiRenderDelegate::~HdAiRenderDelegate() {
    std::lock_guard<std::mutex> guard(_mutexResourceRegistry);
    if (_counterResourceRegistry.fetch_sub(1) == 1) {
        _resourceRegistry.reset();
    }
    _renderParam->Restart();
    hdAiUninstallNodes();
    AiUniverseDestroy(_universe);
    AiEnd();
}

HdRenderParam* HdAiRenderDelegate::GetRenderParam() const {
    return _renderParam.get();
}

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

void HdAiRenderDelegate::SetRenderSetting(
    const TfToken& key, const VtValue& value) {
    if (value.IsHolding<int>()) {
        AiNodeSetInt(_options, key.GetText(), value.UncheckedGet<int>());
    } else if (value.IsHolding<float>()) {
        AiNodeSetFlt(_options, key.GetText(), value.UncheckedGet<float>());
    } else if (value.IsHolding<bool>()) {
        AiNodeSetBool(_options, key.GetText(), value.UncheckedGet<bool>());
    }
    _renderParam->Restart();
}

VtValue HdAiRenderDelegate::GetRenderSetting(const TfToken& key) const {
    const auto* nentry = AiNodeGetNodeEntry(_options);
    const auto* pentry = AiNodeEntryLookUpParameter(nentry, key.GetText());
    if (pentry == nullptr) { return {}; }
    const auto ptype = AiParamGetType(pentry);
    if (ptype == AI_TYPE_INT) {
        // We are changing the default values of these.
        return VtValue(AiNodeGetInt(_options, key.GetText()));
    } else if (ptype == AI_TYPE_FLOAT) {
        return VtValue(AiNodeGetFlt(_options, key.GetText()));
    } else if (ptype == AI_TYPE_BOOLEAN) {
        return VtValue(AiNodeGetBool(_options, key.GetText()));
    }
    return {};
}

// For now we only support a few parameter types, that are expected to have
// UI code in usdview / Maya to Hydra.
HdRenderSettingDescriptorList HdAiRenderDelegate::GetRenderSettingDescriptors()
    const {
    HdRenderSettingDescriptorList ret;
    const auto* nentry = AiNodeGetNodeEntry(_options);
    auto* piter = AiNodeEntryGetParamIterator(nentry);
    while (!AiParamIteratorFinished(piter)) {
        const auto* pentry = AiParamIteratorGetNext(piter);
        const auto pname = AiParamGetName(pentry);
        if (_ignoredParameters.find(pname) != _ignoredParameters.end()) {
            continue;
        }
        const auto ptype = AiParamGetType(pentry);
        HdRenderSettingDescriptor desc;
        if (ptype == AI_TYPE_INT) {
            // We are changing the default values of these.
            if (pname == Str::GI_diffuse_depth ||
                pname == Str::GI_specular_depth) {
                desc.defaultValue = VtValue(1);
            } else {
                desc.defaultValue = VtValue(AiParamGetDefault(pentry)->INT());
            }
        } else if (ptype == AI_TYPE_FLOAT) {
            desc.defaultValue = VtValue(AiParamGetDefault(pentry)->FLT());
        } else if (ptype == AI_TYPE_BOOLEAN) {
            desc.defaultValue = VtValue(AiParamGetDefault(pentry)->BOOL());
        } else {
            continue;
        }
        desc.name = _GetParameterLabel(pname).GetText();
        desc.key = TfToken(pname.c_str());
        ret.push_back(desc);
    }
    AiParamIteratorDestroy(piter);
    return ret;
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
    if (typeId == HdPrimTypeTokens->rectLight) {
        return HdAiLight::CreateRectLight(this, sprimId);
    }
    if (typeId == HdPrimTypeTokens->cylinderLight) {
        return HdAiLight::CreateCylinderLight(this, sprimId);
    }
    if (typeId == HdPrimTypeTokens->domeLight) {
        return HdAiLight::CreateDomeLight(this, sprimId);
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
    if (typeId == HdPrimTypeTokens->rectLight) {
        return HdAiLight::CreateRectLight(this, SdfPath::EmptyPath());
    }
    if (typeId == HdPrimTypeTokens->cylinderLight) {
        return HdAiLight::CreateCylinderLight(this, SdfPath::EmptyPath());
    }
    if (typeId == HdPrimTypeTokens->domeLight) {
        return HdAiLight::CreateDomeLight(this, SdfPath::EmptyPath());
    }
    TF_CODING_ERROR("Unknown Sprim Type %s", typeId.GetText());
    return nullptr;
}

void HdAiRenderDelegate::DestroySprim(HdSprim* sPrim) { delete sPrim; }

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
