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
#include "pxr/imaging/hdAi/mesh.h"

#include "pxr/imaging/hdAi/material.h"
#include "pxr/imaging/hdAi/utils.h"

PXR_NAMESPACE_OPEN_SCOPE

HdAiMesh::HdAiMesh(
    HdAiRenderDelegate* delegate, const SdfPath& id, const SdfPath& instancerId)
    : HdMesh(id, instancerId), _delegate(delegate) {
    _mesh = AiNode(delegate->GetUniverse(), "polymesh");
    AiNodeSetStr(_mesh, "name", id.GetText());
}

void HdAiMesh::Sync(
    HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits, const HdReprSelector& reprSelector,
    bool forcedRepr) {
    TF_UNUSED(forcedRepr);
    const auto& id = GetId();

    if (HdChangeTracker::IsPrimvarDirty(*dirtyBits, id, HdTokens->points)) {
        auto value = sceneDelegate->Get(id, HdTokens->points);
        const auto& vecArray = value.Get<VtVec3fArray>();
        AiNodeSetArray(
            _mesh, "vlist",
            AiArrayConvert(
                vecArray.size(), 1, AI_TYPE_VECTOR, vecArray.data()));
    }

    if (HdChangeTracker::IsTopologyDirty(*dirtyBits, id)) {
        const auto topology = GetMeshTopology(sceneDelegate);
        const auto& vertexCounts = topology.GetFaceVertexCounts();
        const auto& vertexIndices = topology.GetFaceVertexIndices();
        const auto numFaces = topology.GetNumFaces();
        const auto numVertexIndices = vertexIndices.size();
        auto* nsides = AiArrayAllocate(numFaces, 1, AI_TYPE_UINT);
        auto* vidxs = AiArrayAllocate(vertexIndices.size(), 1, AI_TYPE_UINT);
        for (auto i = decltype(numFaces){0}; i < numFaces; ++i) {
            AiArraySetUInt(
                nsides, i, static_cast<unsigned int>(vertexCounts[i]));
        }
        for (auto i = decltype(numVertexIndices){0}; i < numVertexIndices;
             ++i) {
            AiArraySetUInt(
                vidxs, i, static_cast<unsigned int>(vertexIndices[i]));
        }
        AiNodeSetArray(_mesh, "nsides", nsides);
        AiNodeSetArray(_mesh, "vidxs", vidxs);
    }

    if (HdChangeTracker::IsTransformDirty(*dirtyBits, id)) {
        HdAiSetTransform(_mesh, sceneDelegate, GetId());
    }

    if (*dirtyBits & HdChangeTracker::DirtyMaterialId) {
        const auto* material = reinterpret_cast<const HdAiMaterial*>(
            sceneDelegate->GetRenderIndex().GetSprim(
                HdPrimTypeTokens->material, sceneDelegate->GetMaterialId(id)));
        if (material != nullptr) {
            AiNodeSetPtr(_mesh, "shader", material->GetSurfaceShader());
            AiNodeSetPtr(_mesh, "disp_map", material->GetDisplacementShader());
        } else {
            AiNodeSetPtr(_mesh, "shader", _delegate->GetFallbackShader());
            AiNodeSetPtr(_mesh, "disp_map", nullptr);
        }
    }

    if (*dirtyBits & HdChangeTracker::DirtyPrimvar) {
        // TODO: Implement and handle uv / st
    }

    *dirtyBits = ~HdChangeTracker::AllSceneDirtyBits;
}

HdDirtyBits HdAiMesh::GetInitialDirtyBitsMask() const {
    return HdChangeTracker::Clean | HdChangeTracker::InitRepr |
           HdChangeTracker::DirtyPoints | HdChangeTracker::DirtyTopology |
           HdChangeTracker::DirtyTransform | HdChangeTracker::DirtyMaterialId |
           HdChangeTracker::DirtyPrimvar;
}

void HdAiMesh::_UpdateRepr(
    HdSceneDelegate* sceneDelegate, const HdReprSelector& reprSelector,
    HdDirtyBits* dirtyBits) {
    TF_UNUSED(sceneDelegate);
    TF_UNUSED(reprSelector);
    TF_UNUSED(dirtyBits);
}

HdDirtyBits HdAiMesh::_PropagateDirtyBits(HdDirtyBits bits) const {
    return bits & HdChangeTracker::AllDirty;
}

void HdAiMesh::_InitRepr(
    const HdReprSelector& reprSelector, HdDirtyBits* dirtyBits) {
    TF_UNUSED(reprSelector);
    TF_UNUSED(dirtyBits);
}

PXR_NAMESPACE_CLOSE_SCOPE
