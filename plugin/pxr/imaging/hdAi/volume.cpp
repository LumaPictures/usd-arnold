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
#include "pxr/imaging/hdAi/openvdbAsset.h"
#include "pxr/imaging/hdAi/utils.h"

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
    : HdVolume(id, instancerId), _delegate(delegate) {}

HdAiVolume::~HdAiVolume() {
    for (auto& volume : _volumes) { AiNodeDestroy(volume); }
}

void HdAiVolume::Sync(
    HdSceneDelegate* delegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits, const TfToken& reprToken) {
    TF_UNUSED(reprToken);
    auto* param = reinterpret_cast<HdAiRenderParam*>(renderParam);

    const auto& id = GetId();
    auto volumesChanged = false;
    if (HdChangeTracker::IsTopologyDirty(*dirtyBits, id)) {
        param->End();
        _CreateVolumes(id, delegate);
        volumesChanged = true;
    }

    if (volumesChanged || (*dirtyBits & HdChangeTracker::DirtyMaterialId)) {
        param->End();
        const auto* material = reinterpret_cast<const HdAiMaterial*>(
            delegate->GetRenderIndex().GetSprim(
                HdPrimTypeTokens->material, delegate->GetMaterialId(id)));
        if (material != nullptr) {
            auto* surfaceShader = material->GetSurfaceShader();
            for (auto& volume : _volumes) {
                AiNodeSetPtr(volume, Str::shader, surfaceShader);
            }
        }
    }

    if (HdChangeTracker::IsTransformDirty(*dirtyBits, id)) {
        param->End();
        HdAiSetTransform(_volumes, delegate, GetId());
    }

    *dirtyBits = HdChangeTracker::Clean;
}

void HdAiVolume::_CreateVolumes(const SdfPath& id, HdSceneDelegate* delegate) {
    std::unordered_map<std::string, std::vector<TfToken>> openvdbs;
    const auto fieldDescriptors = delegate->GetVolumeFieldDescriptors(id);
    for (const auto& field : fieldDescriptors) {
        auto* openvdbAsset =
            dynamic_cast<HdAiOpenvdbAsset*>(delegate->GetRenderIndex().GetBprim(
                _tokens->openvdbAsset, field.fieldId));
        if (openvdbAsset == nullptr) { continue; }
        openvdbAsset->TrackVolumePrimitive(id);
        const auto vv = delegate->Get(field.fieldId, _tokens->filePath);
        if (vv.IsHolding<SdfAssetPath>()) {
            const auto& assetPath = vv.UncheckedGet<SdfAssetPath>();
            auto path = assetPath.GetResolvedPath();
            if (path.empty()) { path = assetPath.GetAssetPath(); }
            auto& fields = openvdbs[path];
            if (std::find(fields.begin(), fields.end(), field.fieldName) ==
                fields.end()) {
                fields.push_back(field.fieldName);
            }
        }
    }

    _volumes.erase(
        std::remove_if(
            _volumes.begin(), _volumes.end(),
            [&openvdbs](AtNode* node) -> bool {
                if (openvdbs.find(std::string(
                    AiNodeGetStr(node, Str::filename).c_str())) ==
                    openvdbs.end()) {
                    AiNodeDestroy(node);
                    return true;
                }
                return false;
            }),
        _volumes.end());

    for (const auto& openvdb : openvdbs) {
        AtNode* volume = nullptr;
        for (auto& v : _volumes) {
            if (openvdb.first == AiNodeGetStr(v, Str::filename).c_str()) {
                volume = v;
                break;
            }
        }
        if (volume == nullptr) {
            volume = AiNode(_delegate->GetUniverse(), Str::volume);
            AiNodeSetStr(volume, Str::filename, openvdb.first.c_str());
            AiNodeSetStr(
                volume, Str::name,
                id.AppendChild(TfToken(TfStringPrintf("p_%p", volume)))
                    .GetText());
            _volumes.push_back(volume);
        }
        const auto numFields = openvdb.second.size();
        auto* fields = AiArrayAllocate(numFields, 1, AI_TYPE_STRING);
        for (auto i = decltype(numFields){0}; i < numFields; ++i) {
            AiArraySetStr(fields, i, AtString(openvdb.second[i].GetText()));
        }
        AiNodeSetArray(volume, Str::grids, fields);
    }
}

HdDirtyBits HdAiVolume::GetInitialDirtyBitsMask() const {
    return HdChangeTracker::AllDirty;
}

HdDirtyBits HdAiVolume::_PropagateDirtyBits(HdDirtyBits bits) const {
    return bits & HdChangeTracker::AllDirty;
}

void HdAiVolume::_InitRepr(const TfToken& reprToken, HdDirtyBits* dirtyBits) {
    TF_UNUSED(reprToken);
    TF_UNUSED(dirtyBits);
}

PXR_NAMESPACE_CLOSE_SCOPE
