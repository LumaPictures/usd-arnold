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
#include "pxr/imaging/hdAi/light.h"

#include "pxr/imaging/hdAi/material.h"
#include "pxr/imaging/hdAi/utils.h"

#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

namespace {
const AtString pointLightType("point_light");
const AtString distantLightType("distant_light");

struct ParamDesc {
    ParamDesc(const char* aname, const TfToken& hname)
        : arnoldName(aname), hdName(hname) {}
    AtString arnoldName;
    TfToken hdName;
};

std::vector<ParamDesc> genericParams = {
    {"intensity", HdLightTokens->intensity},
    {"exposure", HdLightTokens->exposure},
    {"color", HdLightTokens->color},
    {"diffuse", HdLightTokens->diffuse},
    {"specular", HdLightTokens->specular},
    {"normalize", HdLightTokens->normalize},
    {"cast_shadows", HdLightTokens->shadowEnable},
    {"shadow_color", HdLightTokens->shadowColor},
};

std::vector<ParamDesc> pointParams = {{"radius", HdLightTokens->radius}};

std::vector<ParamDesc> distantParams = {{"angle", HdLightTokens->angle}};

void iterateParams(
    AtNode* light, const AtNodeEntry* nentry, const SdfPath& id,
    HdSceneDelegate* delegate, const std::vector<ParamDesc>& params) {
    for (const auto& param : params) {
        const auto* pentry =
            AiNodeEntryLookUpParameter(nentry, param.arnoldName);
        if (pentry == nullptr) { continue; }
        HdAiMaterial::SetParameter(
            light, pentry, delegate->GetLightParamValue(id, param.hdName));
    }
}

auto pointLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                         const SdfPath& id, HdSceneDelegate* delegate) {
    iterateParams(light, nentry, id, delegate, pointParams);
};

auto distantLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                         const SdfPath& id, HdSceneDelegate* delegate) {
    iterateParams(light, nentry, id, delegate, distantParams);
};

} // namespace

HdAiLight* HdAiLight::CreatePointLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, pointLightType, pointLightSync);
}

HdAiLight* HdAiLight::CreateDistantLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, distantLightType, distantLightSync);
}

void HdAiLight::Sync(
    HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits) {
    TF_UNUSED(sceneDelegate);
    TF_UNUSED(renderParam);
    TF_UNUSED(dirtyBits);
    if (*dirtyBits & HdLight::DirtyParams) {
        const auto id = GetId();
        const auto* nentry = AiNodeGetNodeEntry(_light);
        iterateParams(_light, nentry, id, sceneDelegate, genericParams);
        _syncParams(_light, nentry, id, sceneDelegate);
    }

    if (*dirtyBits & HdLight::DirtyTransform) {
        HdTimeSampleArray<GfMatrix4d, 3> xf;
        sceneDelegate->SampleTransform(GetId(), &xf);
        AtArray* matrices = AiArrayAllocate(1, xf.count, AI_TYPE_MATRIX);
        for (auto i = decltype(xf.count){0}; i < xf.count; ++i) {
            AiArraySetMtx(matrices, i, HdAiConvertMatrix(xf.values[i]));
        }
        AiNodeSetArray(_light, "matrix", matrices);
    }
    *dirtyBits = HdLight::Clean;
}

HdDirtyBits HdAiLight::GetInitialDirtyBitsMask() const {
    return HdLight::DirtyParams | HdLight::DirtyTransform;
}

HdAiLight::HdAiLight(
    HdAiRenderDelegate* delegate, const SdfPath& id, const AtString& arnoldType,
    const HdAiLight::SyncParams& sync)
    : HdLight(id), _syncParams(sync), _delegate(delegate) {
    _light = AiNode(_delegate->GetUniverse(), arnoldType);
    if (id.IsEmpty()) {
        AiNodeSetFlt(_light, "intensity", 0.0f);
    } else {
        AiNodeSetStr(_light, "name", id.GetText());
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
