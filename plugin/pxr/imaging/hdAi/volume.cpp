// Copyright (c) 2019 Luma Pictures . All rights reserved.
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
#include "pxr/imaging/hdAi/volume.h"
#include <pxr/imaging/hd/changeTracker.h>

#include <pxr/usd/sdf/assetPath.h>

#include "pxr/imaging/hdAi/material.h"

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PRIVATE_TOKENS(_tokens, (openvdbAsset)(filePath));

namespace {
namespace Str {
const AtString name("name");
const AtString volume("volume");
const AtString filename("filename");
const AtString grids("grids");
const AtString shader("shader");
} // namespace Str
} // namespace

HdAiVolume::HdAiVolume(
    HdAiRenderDelegate* delegate, const SdfPath& id, const SdfPath& instancerId)
    : HdVolume(id, instancerId), _delegate(delegate) {
    _volume = AiNode(delegate->GetUniverse(), Str::volume);
    AiNodeSetStr(_volume, Str::name, id.GetText());
}

HdAiVolume::~HdAiVolume() { AiNodeDestroy(_volume); }

void HdAiVolume::Sync(
    HdSceneDelegate* delegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits, const TfToken& reprToken) {
    TF_UNUSED(dirtyBits);
    TF_UNUSED(reprToken);
    auto* param = reinterpret_cast<HdAiRenderParam*>(renderParam);
    param->End();
    const auto& id = GetId();

    std::unordered_map<SdfAssetPath, std::vector<TfToken>, SdfAssetPath::Hash>
        openvdbs;
    const auto fieldDescriptors = delegate->GetVolumeFieldDescriptors(id);
    for (const auto& field : fieldDescriptors) {
        const auto vv = delegate->Get(field.fieldId, _tokens->filePath);
        if (vv.IsHolding<SdfAssetPath>()) {
            auto& fields = openvdbs[vv.UncheckedGet<SdfAssetPath>()];
            if (std::find(fields.begin(), fields.end(), field.fieldName) ==
                fields.end()) {
                fields.push_back(field.fieldName);
            }
        }
    }

    // TODO: support multiple filepaths.
    if (!openvdbs.empty()) {
        const auto& it = openvdbs.cbegin();
        auto path = it->first.GetResolvedPath();
        if (path.empty()) { path = it->first.GetAssetPath(); }
        AiNodeSetStr(_volume, Str::filename, path.c_str());
        const auto numFields = it->second.size();
        auto* fields = AiArrayAllocate(numFields, 1, AI_TYPE_STRING);
        for (auto i = decltype(numFields){0}; i < numFields; ++i) {
            AiArraySetStr(fields, i, AtString(it->second[i].GetText()));
        }
        AiNodeSetArray(_volume, Str::grids, fields);
    }

    const auto* material = reinterpret_cast<const HdAiMaterial*>(
        delegate->GetRenderIndex().GetSprim(
            HdPrimTypeTokens->material, delegate->GetMaterialId(id)));
    if (material != nullptr) {
        AiNodeSetPtr(_volume, Str::shader, material->GetSurfaceShader());
    }

    *dirtyBits = ~HdChangeTracker::AllSceneDirtyBits;
}

HdDirtyBits HdAiVolume::GetInitialDirtyBitsMask() const {
    return HdChangeTracker::AllSceneDirtyBits;
}

HdDirtyBits HdAiVolume::_PropagateDirtyBits(HdDirtyBits bits) const {
    return bits & HdChangeTracker::AllDirty;
}

void HdAiVolume::_InitRepr(const TfToken& reprToken, HdDirtyBits* dirtyBits) {
    TF_UNUSED(reprToken);
    TF_UNUSED(dirtyBits);
}

PXR_NAMESPACE_CLOSE_SCOPE
