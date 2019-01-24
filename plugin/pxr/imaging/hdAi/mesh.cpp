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
#include "pxr/imaging/hdAi/mesh.h"

#include <pxr/base/gf/vec2f.h>

#include <pxr/imaging/hdAi/material.h>
#include <pxr/imaging/hdAi/utils.h>

#include <pxr/imaging/pxOsd/tokens.h>

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PRIVATE_TOKENS(_tokens, (st)(uv));

namespace {
namespace Str {
const AtString name("name");
const AtString polymesh("polymesh");
const AtString vlist("vlist");
const AtString vidxs("vidxs");
const AtString nsides("nsides");
const AtString uvlist("uvlist");
const AtString uvidxs("uvidxs");
const AtString shader("shader");
const AtString disp_map("disp_map");
const AtString opaque("opaque");
const AtString subdiv_type("subdiv_type");
const AtString catclark("catclark");
const AtString none("none");
const AtString subdiv_iterations("subdiv_iterations");
const AtString crease_idxs("crease_idxs");
const AtString crease_sharpness("crease_sharpness");

} // namespace Str

} // namespace

HdAiMesh::HdAiMesh(
    HdAiRenderDelegate* delegate, const SdfPath& id, const SdfPath& instancerId)
    : HdMesh(id, instancerId), _delegate(delegate) {
    _mesh = AiNode(delegate->GetUniverse(), Str::polymesh);
    AiNodeSetStr(_mesh, Str::name, id.GetText());
    // The default value is 1, which won't work well in a Hydra context.
    AiNodeSetByte(_mesh, Str::subdiv_iterations, 0);
}

HdAiMesh::~HdAiMesh() { AiNodeDestroy(_mesh); }

void HdAiMesh::Sync(
    HdSceneDelegate* delegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits, const TfToken& reprToken) {
    auto* param = reinterpret_cast<HdAiRenderParam*>(renderParam);
    const auto& id = GetId();

    if (HdChangeTracker::IsPrimvarDirty(*dirtyBits, id, HdTokens->points)) {
        param->End();
        constexpr size_t maxSamples = 2;
        HdTimeSampleArray<VtValue, maxSamples> xf;
        delegate->SamplePrimvar(id, HdTokens->points, &xf);
        if (xf.count > 0 &&
            ARCH_LIKELY(xf.values[0].IsHolding<VtVec3fArray>())) {
            const auto& v0 = xf.values[0].Get<VtVec3fArray>();
            if (xf.count > 1 &&
                ARCH_UNLIKELY(!xf.values[1].IsHolding<VtVec3fArray>())) {
                xf.count = 1;
            }
            auto* arr = AiArrayAllocate(v0.size(), xf.count, AI_TYPE_VECTOR);
            AiArraySetKey(arr, 0, v0.data());
            if (xf.count > 1) {
                const auto& v1 = xf.values[1].Get<VtVec3fArray>();
                if (ARCH_LIKELY(v1.size() == v0.size())) {
                    AiArraySetKey(arr, 1, v1.data());
                } else {
                    AiArraySetKey(arr, 1, v0.data());
                }
            }
            AiNodeSetArray(_mesh, Str::vlist, arr);
        }
    }

    if (HdChangeTracker::IsTopologyDirty(*dirtyBits, id)) {
        param->End();
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
        AiNodeSetArray(_mesh, Str::nsides, nsides);
        AiNodeSetArray(_mesh, Str::vidxs, vidxs);
        const auto scheme = topology.GetScheme();
        if (scheme == PxOsdOpenSubdivTokens->catmullClark ||
            scheme == PxOsdOpenSubdivTokens->catmark) {
            AiNodeSetStr(_mesh, Str::subdiv_type, Str::catclark);
        } else {
            AiNodeSetStr(_mesh, Str::subdiv_type, Str::none);
        }
    }

    if (HdChangeTracker::IsDisplayStyleDirty(*dirtyBits, id)) {
        const auto displayStyle = GetDisplayStyle(delegate);
        AiNodeSetByte(
            _mesh, Str::subdiv_iterations,
            static_cast<uint8_t>(std::max(0, displayStyle.refineLevel)));
    }

    if (HdChangeTracker::IsTransformDirty(*dirtyBits, id)) {
        param->End();
        HdAiSetTransform(_mesh, delegate, GetId());
    }

    if (HdChangeTracker::IsSubdivTagsDirty(*dirtyBits, id)) {
        const auto subdivTags = GetSubdivTags(delegate);
        const auto& cornerIndices = subdivTags.GetCornerIndices();
        const auto& cornerWeights = subdivTags.GetCornerWeights();
        const auto& creaseIndices = subdivTags.GetCreaseIndices();
        const auto& creaseLengths = subdivTags.GetCreaseLengths();
        const auto& creaseWeights = subdivTags.GetCreaseWeights();

        const auto cornerIndicesCount =
            static_cast<uint32_t>(cornerIndices.size());
        uint32_t cornerWeightCounts = 0;
        for (auto creaseLength : creaseLengths) {
            cornerWeightCounts += std::max(0, creaseLength - 1);
        }

        const auto creaseIdxsCount =
            cornerIndicesCount * 2 + cornerWeightCounts * 2;
        const auto craseSharpnessCount =
            cornerIndicesCount + cornerWeightCounts;

        auto* creaseIdxs = AiArrayAllocate(creaseIdxsCount, 1, AI_TYPE_UINT);
        auto* creaseSharpness =
            AiArrayAllocate(craseSharpnessCount, 1, AI_TYPE_FLOAT);

        uint32_t ii = 0;
        for (auto cornerIndex : cornerIndices) {
            AiArraySetUInt(creaseIdxs, ii * 2, cornerIndex);
            AiArraySetUInt(creaseIdxs, ii * 2 + 1, cornerIndex);
            AiArraySetFlt(creaseSharpness, ii, cornerWeights[ii]);
            ++ii;
        }

        uint32_t jj = 0;
        for (auto creaseLength : creaseLengths) {
            for (auto k = decltype(creaseLength){1}; k < creaseLength;
                 ++k, ++ii) {
                AiArraySetUInt(creaseIdxs, ii * 2, creaseIndices[jj + k - 1]);
                AiArraySetUInt(creaseIdxs, ii * 2 + 1, creaseIndices[jj + k]);
                AiArraySetFlt(creaseSharpness, ii, creaseWeights[jj]);
            }
            jj += creaseLength;
        }

        AiNodeSetArray(_mesh, Str::crease_idxs, creaseIdxs);
        AiNodeSetArray(_mesh, Str::crease_sharpness, creaseSharpness);
    }

    if (*dirtyBits & HdChangeTracker::DirtyMaterialId) {
        param->End();
        const auto* material = reinterpret_cast<const HdAiMaterial*>(
            delegate->GetRenderIndex().GetSprim(
                HdPrimTypeTokens->material, delegate->GetMaterialId(id)));
        if (material != nullptr) {
            AiNodeSetPtr(_mesh, Str::shader, material->GetSurfaceShader());
            AiNodeSetPtr(
                _mesh, Str::disp_map, material->GetDisplacementShader());
            // TODO: We need a way to detect this.
            AiNodeSetBool(_mesh, Str::opaque, false);
        } else {
            AiNodeSetPtr(_mesh, Str::shader, _delegate->GetFallbackShader());
            AiNodeSetPtr(_mesh, Str::disp_map, nullptr);
        }
    }

    // TODO: Implement all the primvars.
    if (*dirtyBits & HdChangeTracker::DirtyPrimvar) {
        param->End();
        for (const auto& primvar : delegate->GetPrimvarDescriptors(
                 id, HdInterpolation::HdInterpolationConstant)) {
            HdAiSetConstantPrimvar(_mesh, id, delegate, primvar);
        }
        for (const auto& primvar : delegate->GetPrimvarDescriptors(
                 id, HdInterpolation::HdInterpolationUniform)) {
            HdAiSetUniformPrimvar(_mesh, id, delegate, primvar);
        }
        for (const auto& primvar : delegate->GetPrimvarDescriptors(
                 id, HdInterpolation::HdInterpolationVertex)) {
            if (primvar.name == HdTokens->points) { continue; }
            HdAiSetVertexPrimvar(_mesh, id, delegate, primvar);
        }
        for (const auto& primvar : delegate->GetPrimvarDescriptors(
                 id, HdInterpolation::HdInterpolationFaceVarying)) {
            if (primvar.name == _tokens->st || primvar.name == _tokens->uv) {
                const auto v = delegate->Get(id, primvar.name);
                if (v.IsHolding<VtArray<GfVec2f>>()) {
                    const auto& uv = v.UncheckedGet<VtArray<GfVec2f>>();
                    const auto numUVs = static_cast<unsigned int>(uv.size());
                    // Same memory layout and this data is flattened.
                    auto* uvlist =
                        AiArrayConvert(numUVs, 1, AI_TYPE_VECTOR2, uv.data());
                    auto* uvidxs = AiArrayAllocate(numUVs, 1, AI_TYPE_UINT);
                    for (auto i = decltype(numUVs){0}; i < numUVs; ++i) {
                        AiArraySetUInt(uvidxs, i, i);
                    }
                    AiNodeSetArray(_mesh, Str::uvlist, uvlist);
                    AiNodeSetArray(_mesh, Str::uvidxs, uvidxs);
                }
            } else {
                HdAiSetFaceVaryingPrimvar(_mesh, id, delegate, primvar);
            }
        }
    }

    *dirtyBits = HdChangeTracker::Clean;
}

HdDirtyBits HdAiMesh::GetInitialDirtyBitsMask() const {
    return HdChangeTracker::Clean | HdChangeTracker::InitRepr |
           HdChangeTracker::DirtyPoints | HdChangeTracker::DirtyTopology |
           HdChangeTracker::DirtyTransform | HdChangeTracker::DirtyMaterialId |
           HdChangeTracker::DirtyPrimvar;
}

HdDirtyBits HdAiMesh::_PropagateDirtyBits(HdDirtyBits bits) const {
    return bits & HdChangeTracker::AllDirty;
}

void HdAiMesh::_InitRepr(const TfToken& reprToken, HdDirtyBits* dirtyBits) {
    TF_UNUSED(reprToken);
    TF_UNUSED(dirtyBits);
}

PXR_NAMESPACE_CLOSE_SCOPE
