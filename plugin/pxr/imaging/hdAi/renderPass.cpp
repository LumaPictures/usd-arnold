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
/*
 * TODO:
 * - Restart rendering if proj / view mtx or the viewport changes.
 * - Writing to the render buffers directly.
 * - Handling the depth buffer.
 */
#include "pxr/imaging/hdAi/renderPass.h"

#include <pxr/imaging/hd/renderPassState.h>

#include "pxr/imaging/hdAi/nodes/nodes.h"

PXR_NAMESPACE_OPEN_SCOPE

HdAiRenderPass::HdAiRenderPass(
    AtUniverse* universe, HdRenderIndex* index,
    const HdRprimCollection& collection)
    : HdRenderPass(index, collection) {
    _camera = AiNode(universe, HdAiNodeNames::camera);
    AiNodeSetPtr(AiUniverseGetOptions(universe), "camera", _camera);
    AiNodeSetStr(_camera, "name", "HdAiRenderPass_camera");
    _filter = AiNode(universe, "gaussian_filter");
    AiNodeSetStr(_filter, "name", "HdAiRenderPass_filter");
    _driver = AiNode(universe, HdAiNodeNames::driver);
    AiNodeSetStr(_driver, "name", "HdAiRenderPass_driver");
    _options = AiUniverseGetOptions(universe);
}

void HdAiRenderPass::_Execute(
    const HdRenderPassStateSharedPtr& renderPassState,
    const TfTokenVector& renderTags) {
    if (AiRendering()) { AiRenderAbort(AI_BLOCKING); }

    auto convertMtx = [](const GfMatrix4d& in) -> AtMatrix {
        AtMatrix out = AI_M4_IDENTITY;
        for (auto i = 0; i < 16; ++i) {
            *(&out.data[0][0] + i) = static_cast<float>(*(in.data() + i));
        }
        return out;
    };

    AiNodeSetMatrix(
        _camera, HdAiCamera::projMtx,
        convertMtx(renderPassState->GetProjectionMatrix()));
    AiNodeSetMatrix(
        _camera, "matrix", convertMtx(renderPassState->GetWorldToViewMatrix()));

    const auto vp = renderPassState->GetViewport();

    _width = static_cast<unsigned int>(vp[2]);
    _height = static_cast<unsigned int>(vp[3]);

    AiRender();

#pragma pack(1)
    struct Color {
        uint8_t r, g, b, a;
    };
#pragma pack()

    // Blitting to the OpenGL buffer.
    std::vector<Color> color(_width * _height, {255, 0, 0, 255});
    std::vector<float> depth(_width * _height, 1.0f - AI_EPSILON);
    _compositor.UpdateColor(
        _width, _height, reinterpret_cast<uint8_t*>(color.data()));
    _compositor.UpdateDepth(
        _width, _height, reinterpret_cast<uint8_t*>(depth.data()));
    _compositor.Draw();
}

PXR_NAMESPACE_CLOSE_SCOPE
