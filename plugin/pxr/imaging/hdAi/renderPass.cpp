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
/*
 * TODO:
 * - Writing to the render buffers directly.
 */
#include "pxr/imaging/hdAi/renderPass.h"

#include <pxr/imaging/hd/renderPassState.h>

#include "pxr/imaging/hdAi/config.h"
#include "pxr/imaging/hdAi/nodes/nodes.h"
#include "pxr/imaging/hdAi/utils.h"

#include <algorithm>
#include <cstring> // memset

namespace {
namespace Str {
const AtString renderPassCamera("HdAiRenderPass_camera");
const AtString renderPassFilter("HdAiRenderPass_beautyFilter");
const AtString renderPassClosestFilter("HdAiRenderPass_closestFilter");
const AtString renderPassDriver("HdAiRenderPass_driver");

const AtString persp_camera("persp_camera");
const AtString camera("camera");
const AtString matrix("matrix");
const AtString name("name");
const AtString gaussian_filter("gaussian_filter");
const AtString closest_filter("closest_filter");
const AtString outputs("outputs");
const AtString shutter_start("shutter_start");
const AtString shutter_end("shutter_end");
const AtString fov("fov");
const AtString xres("xres");
const AtString yres("yres");
} // namespace Str
} // namespace

PXR_NAMESPACE_OPEN_SCOPE

HdAiRenderPass::HdAiRenderPass(
    HdAiRenderDelegate* delegate, HdRenderIndex* index,
    const HdRprimCollection& collection)
    : HdRenderPass(index, collection), _delegate(delegate) {
    auto* universe = _delegate->GetUniverse();
    _camera = AiNode(universe, Str::persp_camera);
    AiNodeSetPtr(AiUniverseGetOptions(universe), Str::camera, _camera);
    AiNodeSetStr(
        _camera, Str::name, _delegate->GetLocalNodeName(Str::renderPassCamera));
    _beautyFilter = AiNode(universe, Str::gaussian_filter);
    AiNodeSetStr(
        _beautyFilter, Str::name,
        _delegate->GetLocalNodeName(Str::renderPassFilter));
    _closestFilter = AiNode(universe, Str::closest_filter);
    AiNodeSetStr(
        _closestFilter, Str::name,
        _delegate->GetLocalNodeName(Str::renderPassClosestFilter));
    _driver = AiNode(universe, HdAiNodeNames::driver);
    AiNodeSetStr(
        _driver, Str::name, _delegate->GetLocalNodeName(Str::renderPassDriver));
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
    AiNodeSetArray(options, Str::outputs, outputsArray);

    const auto& config = HdAiConfig::GetInstance();
    AiNodeSetFlt(_camera, Str::shutter_start, config.shutter_start);
    AiNodeSetFlt(_camera, Str::shutter_end, config.shutter_end);
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

    const auto projMtx = renderPassState->GetProjectionMatrix();
    const auto viewMtx = renderPassState->GetWorldToViewMatrix();
    auto restarted = false;
    if (projMtx != _projMtx || viewMtx != _viewMtx) {
        _projMtx = projMtx;
        _viewMtx = viewMtx;
        renderParam->Restart();
        restarted = true;
        AiNodeSetMatrix(
            _camera, Str::matrix, HdAiConvertMatrix(_viewMtx.GetInverse()));
        AiNodeSetMatrix(
            _driver, HdAiDriver::projMtx, HdAiConvertMatrix(_projMtx));
        AiNodeSetMatrix(
            _driver, HdAiDriver::viewMtx, HdAiConvertMatrix(_viewMtx));
        const auto fov = static_cast<float>(
            GfRadiansToDegrees(atan(1.0 / _projMtx[0][0]) * 2.0));
        AiNodeSetFlt(_camera, Str::fov, fov);
    }

    const auto width = static_cast<int>(vp[2]);
    const auto height = static_cast<int>(vp[3]);
    const auto numPixels = static_cast<size_t>(width * height);
    if (width != _width || height != _height) {
        if (!restarted) { renderParam->Restart(); }
        hdAiEmptyBucketQueue([](const HdAiBucketData*) {});
        const auto oldNumPixels = static_cast<size_t>(_width * _height);
        _width = width;
        _height = height;

        auto* options = _delegate->GetOptions();
        AiNodeSetInt(options, Str::xres, _width);
        AiNodeSetInt(options, Str::yres, _height);

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
