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
#include "pxr/imaging/hdAi/utils.h"

#include <algorithm>

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

void HdAiRenderPass::_Execute(
    const HdRenderPassStateSharedPtr& renderPassState,
    const TfTokenVector& renderTags) {
    auto* renderParam =
        reinterpret_cast<HdAiRenderParam*>(_delegate->GetRenderParam());
    const auto vp = renderPassState->GetViewport();

    const auto projMtx = renderPassState->GetProjectionMatrix();
    const auto viewMtx = renderPassState->GetWorldToViewMatrix();
    if (projMtx != _projMtx || viewMtx != _viewMtx) {
        _projMtx = projMtx;
        _viewMtx = viewMtx;
        _viewInvMtx = _viewMtx.GetInverse();
        renderParam->StopRender();
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
        renderParam->StopRender();
        hdAiEmptyBucketQueue([](const HdAiBucketData*) {});
        const auto oldNumPixels = static_cast<size_t>(_width * _height);
        const auto oldNumPixelBytes = oldNumPixels * 4;
        _width = width;
        _height = height;
        AiNodeSetFlt(_camera, HdAiCamera::frameAspect, vp[2] / vp[3]);

        auto* options = _delegate->GetOptions();
        AiNodeSetInt(options, "xres", _width);
        AiNodeSetInt(options, "yres", _height);

        AiNodeSetInt(options, "bucket_size", 24);

        const auto numPixelBytes = numPixels * 4;
        if (oldNumPixels < numPixels) {
            _colorBuffer.resize(numPixelBytes, 0);
            _depthBuffer.resize(numPixels, 1.0f);
            std::fill(
                _colorBuffer.begin(), _colorBuffer.begin() + oldNumPixelBytes,
                0);
            std::fill(
                _depthBuffer.begin(), _depthBuffer.begin() + oldNumPixels,
                1.0f);
        } else if (oldNumPixels > numPixels) {
            _colorBuffer.resize(numPixelBytes);
            _depthBuffer.resize(numPixels);
            std::fill(
                _colorBuffer.begin(), _colorBuffer.begin() + numPixelBytes, 0);
            std::fill(
                _depthBuffer.begin(), _depthBuffer.begin() + numPixels, 1.0f);
        } else {
            std::fill(_colorBuffer.begin(), _colorBuffer.end(), 0);
            std::fill(_depthBuffer.begin(), _depthBuffer.end(), 1.0f);
        }
    }

    _isConverged = renderParam->Render();

    hdAiEmptyBucketQueue([this](const HdAiBucketData* data) {
        const auto xo = AiClamp(data->xo, 0, _width - 1);
        const auto xe = AiClamp(data->xo + data->sizeX, 0, _width - 1);
        if (xe == xo) { return; }
        const auto yo = AiClamp(data->yo, 0, _height - 1);
        const auto ye = AiClamp(data->yo + data->sizeY, 0, _height - 1);
        if (ye == yo) { return; }
        {
            constexpr auto numChannels = 4;
            const auto pixelSizeOut = sizeof(uint8_t) * numChannels;
            for (auto y = yo; y < ye; ++y) {
                const auto* strideIn =
                    data->beauty.data() + data->sizeX * (y - data->yo);
                auto* strideOut = _colorBuffer.data() +
                                  pixelSizeOut * _width * (_height - y - 1);
                for (auto x = xo; x < xe; ++x) {
                    const auto* in = strideIn + (x - data->xo);
                    auto* out = strideOut + numChannels * x;
                    out[0] = AiQuantize8bit(x + xo, y + yo, 0, in->r, true);
                    out[1] = AiQuantize8bit(x + xo, y + yo, 1, in->g, true);
                    out[2] = AiQuantize8bit(x + xo, y + yo, 2, in->b, true);
                    out[3] = AiQuantize8bit(x + xo, y + yo, 3, in->a, true);
                }
            }
        }
        {
            for (auto y = yo; y < ye; ++y) {
                const auto* strideIn = reinterpret_cast<const float*>(
                    data->depth.data() + data->sizeX * (y - data->yo));
                auto* strideOut =
                    _depthBuffer.data() + _width * (_height - y - 1);
                for (auto x = xo; x < xe; ++x) {
                    strideOut[x] = strideIn[x - data->xo];
                }
            }
        }
    });

    // TODO: this needs to be updated only if data have changed.
    _compositor.UpdateColor(_width, _height, _colorBuffer.data());
    _compositor.UpdateDepth(
        _width, _height, reinterpret_cast<uint8_t*>(_depthBuffer.data()));
    _compositor.Draw();
}

PXR_NAMESPACE_CLOSE_SCOPE
