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
    bool forcedRepr) {}

HdDirtyBits HdAiMesh::GetInitialDirtyBitsMask() const {
    return HdChangeTracker::AllDirty;
}

void HdAiMesh::_UpdateRepr(
    HdSceneDelegate* sceneDelegate, const HdReprSelector& reprSelector,
    HdDirtyBits* dirtyBits) {
    *dirtyBits = HdChangeTracker::Clean;
}

HdDirtyBits HdAiMesh::_PropagateDirtyBits(HdDirtyBits bits) const {
    return bits & HdChangeTracker::AllDirty;
}

void HdAiMesh::_InitRepr(
    const HdReprSelector& reprSelector, HdDirtyBits* dirtyBits) {
    *dirtyBits = HdChangeTracker::Clean;
}

PXR_NAMESPACE_CLOSE_SCOPE
