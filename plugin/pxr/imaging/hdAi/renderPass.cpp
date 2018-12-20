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
 * - Writing to the render buffers directly.
 */
#include "pxr/imaging/hdAi/renderPass.h"

#include <pxr/imaging/hd/renderPassState.h>

#include "pxr/imaging/hdAi/nodes/nodes.h"
#include "pxr/imaging/hdAi/utils.h"

#include <algorithm>
#include <cstring> // memset

namespace {
const AtString cameraName("HdAiRenderPass_camera");
const AtString filterName("HdAiRenderPass_beautyFilter");
const AtString closestName("HdAiRenderPass_closestFilter");
const AtString driverName("HdAiRenderPass_driver");
} // namespace

PXR_NAMESPACE_OPEN_SCOPE

HdAiRenderPass::HdAiRenderPass(
    HdAiRenderDelegate* delegate, HdRenderIndex* index,
    const HdRprimCollection& collection)
    : HdRenderPass(index, collection), _delegate(delegate) {
    auto* universe = _delegate->GetUniverse();
    _camera = AiNode(universe, HdAiNodeNames::camera);
    AiNodeSetPtr(AiUniverseGetOptions(universe), "camera", _camera);
    AiNodeSetStr(_camera, "name", _delegate->GetLocalNodeName(cameraName));
    _beautyFilter = AiNode(universe, "gaussian_filter");
    AiNodeSetStr(
        _beautyFilter, "name", _delegate->GetLocalNodeName(filterName));
    _closestFilter = AiNode(universe, "closest_filter");
    AiNodeSetStr(
        _closestFilter, "name", _delegate->GetLocalNodeName(closestName));
    _driver = AiNode(universe, HdAiNodeNames::driver);
    AiNodeSetStr(_driver, "name", _delegate->GetLocalNodeName(driverName));
    auto* options = _delegate->GetOptions();
    auto* outputsArray = AiArrayAllocate(2, 1, AI_TYPE_STRING);
    const auto beautyString = TfStringPrintf(
        "RGBA RGBA %s %s", AiNodeGetName(_beautyFilter),
        AiNodeGetName(_driver));
    // We need NDC, and the easiest way is to use the position.
    const auto positionString = TfStringPrintf(
        "P VECTOR %s %s", AiNodeGetName(_closestFilter),
        AiNodeGetName(_driver));
    AiArraySetStr(outputsArray, 0, beautyString.c_str());
    AiArraySetStr(outputsArray, 1, positionString.c_str());
    AiNodeSetArray(options, "outputs", outputsArray);

    AiNodeSetFlt(_camera, "shutter_start", -0.25f);
    AiNodeSetFlt(_camera, "shutter_end", 0.25f);
}

HdAiRenderPass::~HdAiRenderPass() {
    AiNodeDestroy(_camera);
    AiNodeDestroy(_beautyFilter);
    AiNodeDestroy(_closestFilter);
    AiNodeDestroy(_driver);
}

void HdAiRenderPass::_Execute(
    const HdRenderPassStateSharedPtr& renderPassState,
    const TfTokenVector& renderTags) {
    auto* renderParam =
        reinterpret_cast<HdAiRenderParam*>(_delegate->GetRenderParam());
    const auto vp = renderPassState->GetViewport();

    auto ended = false;
    const auto projMtx = renderPassState->GetProjectionMatrix();
    const auto viewMtx = renderPassState->GetWorldToViewMatrix();
    if (projMtx != _projMtx || viewMtx != _viewMtx) {
        _projMtx = projMtx;
        _viewMtx = viewMtx;
        _viewInvMtx = _viewMtx.GetInverse();
        renderParam->End();
        ended = true;
        AiNodeSetMatrix(
            _camera, HdAiCamera::projMtx, HdAiConvertMatrix(_projMtx));
        AiNodeSetMatrix(_camera, "matrix", HdAiConvertMatrix(_viewInvMtx));
        AiNodeSetMatrix(
            _driver, HdAiDriver::projMtx, HdAiConvertMatrix(_projMtx));
        AiNodeSetMatrix(
            _driver, HdAiDriver::viewMtx, HdAiConvertMatrix(_viewMtx));
    }

    const auto width = static_cast<int>(vp[2]);
    const auto height = static_cast<int>(vp[3]);
    const auto numPixels = static_cast<size_t>(width * height);
    if (width != _width || height != _height) {
        if (!ended) { renderParam->End(); }
        hdAiEmptyBucketQueue([](const HdAiBucketData*) {});
        const auto oldNumPixels = static_cast<size_t>(_width * _height);
        _width = width;
        _height = height;
        AiNodeSetFlt(_camera, HdAiCamera::frameAspect, vp[2] / vp[3]);

        auto* options = _delegate->GetOptions();
        AiNodeSetInt(options, "xres", _width);
        AiNodeSetInt(options, "yres", _height);

        AiNodeSetInt(options, "bucket_size", 24);

        if (oldNumPixels < numPixels) {
            _colorBuffer.resize(numPixels, AtRGBA8());
            _depthBuffer.resize(numPixels, 1.0f);
            memset(_colorBuffer.data(), 0, oldNumPixels * sizeof(AtRGBA8));
            std::fill(
                _depthBuffer.begin(), _depthBuffer.begin() + oldNumPixels,
                1.0f);
        } else {
            if (numPixels != oldNumPixels) {
                _colorBuffer.resize(numPixels);
                _depthBuffer.resize(numPixels);
            }
            memset(_colorBuffer.data(), 0, numPixels * sizeof(AtRGBA8));
            std::fill(_depthBuffer.begin(), _depthBuffer.end(), 1.0f);
        }
    }

    _isConverged = renderParam->Render();

    bool needsUpdate = false;
    hdAiEmptyBucketQueue([this, &needsUpdate](const HdAiBucketData* data) {
        const auto xo = AiClamp(data->xo, 0, _width - 1);
        const auto xe = AiClamp(data->xo + data->sizeX, 0, _width - 1);
        if (xe == xo) { return; }
        const auto yo = AiClamp(data->yo, 0, _height - 1);
        const auto ye = AiClamp(data->yo + data->sizeY, 0, _height - 1);
        if (ye == yo) { return; }
        needsUpdate = true;
        const auto beautyWidth = (xe - xo) * sizeof(AtRGBA8);
        const auto depthWidth = (xe - xo) * sizeof(float);
        const auto inOffsetG = xo - data->xo - data->sizeX * data->yo;
        const auto outOffsetG = _width * (_height - 1);
        for (auto y = yo; y < ye; ++y) {
            const auto inOffset = data->sizeX * y + inOffsetG;
            const auto outOffset = xo + outOffsetG - _width * y;
            memcpy(
                _colorBuffer.data() + outOffset, data->beauty.data() + inOffset,
                beautyWidth);
            memcpy(
                _depthBuffer.data() + outOffset, data->depth.data() + inOffset,
                depthWidth);
        }
    });

    // If the buffers are empty, needsUpdate will be false.
    if (needsUpdate) {
        _compositor.UpdateColor(
            _width, _height, reinterpret_cast<uint8_t*>(_colorBuffer.data()));
        _compositor.UpdateDepth(
            _width, _height, reinterpret_cast<uint8_t*>(_depthBuffer.data()));
    }
    _compositor.Draw();
}

PXR_NAMESPACE_CLOSE_SCOPE
