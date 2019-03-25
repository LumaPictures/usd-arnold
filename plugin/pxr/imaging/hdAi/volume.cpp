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
