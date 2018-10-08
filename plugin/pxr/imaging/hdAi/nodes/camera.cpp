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
#include <ai.h>

/*
 * TODO:
 * - Make sure derivatives are calculated properly.
 * - Investigate AiCameraUpdate's second parameter.
 */

AI_CAMERA_NODE_EXPORT_METHODS(HdAiCameraMtd)

namespace {

// Arnold's camera operates in view space, so we only need to
// deal with the projection matrix.
AtString _projMtxName("projMtx");

struct ShaderData {
    AtMatrix projMtx;
    AtMatrix projMtxInv;
};

} // namespace

node_parameters { AiParameterMtx(_projMtxName, AtMatrix()); }

node_initialize {
    AiCameraInitialize(node);
    AiNodeSetLocalData(node, new ShaderData());
}

node_update {
    AiCameraUpdate(node, false);
    auto* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    data->projMtx = AiNodeGetMatrix(node, _projMtxName);
    data->projMtxInv = AiM4Invert(data->projMtx);
}

node_finish { delete reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node)); }

camera_create_ray {
    const auto* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    output.weight = AI_RGB_WHITE;

    AtVector ndc(input.sx, input.sy, -1.0f);
    auto npt = AiM4VectorByMatrixMult(data->projMtxInv, ndc);
    if (fabsf(npt.z) < AI_EPSILON) {
        output.origin = npt;
        output.dir = AI_V3_NEGZ;
    } else {
        output.origin = AI_V3_ZERO;
        output.dir = AiV3Normalize(npt);
    }
}

camera_reverse_ray {
    const auto* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    const auto ppo = AiM4VectorByMatrixMult(data->projMtx, Po);
    Ps.x = ppo.x / ppo.z;
    Ps.y = ppo.y / ppo.z;
    return true;
}
