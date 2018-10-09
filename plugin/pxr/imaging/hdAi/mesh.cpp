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

#include "pxr/imaging/hdAi/utils.h"

PXR_NAMESPACE_OPEN_SCOPE

HdAiMesh::HdAiMesh(
    AtUniverse* universe, const SdfPath& id, const SdfPath& instancerId)
    : HdMesh(id, instancerId) {
    _mesh = AiNode(universe, "polymesh");
    AiNodeSetStr(_mesh, "name", id.GetText());
}

void HdAiMesh::Sync(
    HdSceneDelegate* delegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits, const HdReprSelector& reprSelector,
    bool forcedRepr) {
    const auto& id = GetId();

    if (HdChangeTracker::IsPrimvarDirty(*dirtyBits, id, HdTokens->points)) {
        auto value = delegate->Get(id, HdTokens->points);
        const auto& vecArray = value.Get<VtVec3fArray>();
        AiNodeSetArray(
            _mesh, "vlist",
            AiArrayConvert(
                vecArray.size(), 1, AI_TYPE_VECTOR, vecArray.data()));
    }

    if (HdChangeTracker::IsTopologyDirty(*dirtyBits, id)) {
        const auto topology = GetMeshTopology(delegate);
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
        const auto transform = delegate->GetTransform(id);
        AiNodeSetMatrix(_mesh, "matrix", HdAiConvertMatrix(transform));
    }

    *dirtyBits = ~HdChangeTracker::AllSceneDirtyBits;
}

HdDirtyBits HdAiMesh::GetInitialDirtyBitsMask() const {
    return HdChangeTracker::Clean | HdChangeTracker::InitRepr |
           HdChangeTracker::DirtyPoints | HdChangeTracker::DirtyTopology |
           HdChangeTracker::DirtyTransform;
}

void HdAiMesh::_UpdateRepr(
    HdSceneDelegate* sceneDelegate, const HdReprSelector& reprSelector,
    HdDirtyBits* dirtyBits) {}

HdDirtyBits HdAiMesh::_PropagateDirtyBits(HdDirtyBits bits) const {
    return bits & HdChangeTracker::AllDirty;
}

void HdAiMesh::_InitRepr(
    const HdReprSelector& reprSelector, HdDirtyBits* dirtyBits) {}

PXR_NAMESPACE_CLOSE_SCOPE
