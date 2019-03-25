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
#ifndef HDAI_RENDER_PASS_H
#define HDAI_RENDER_PASS_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/base/gf/matrix4d.h>
#include <pxr/imaging/hd/renderPass.h>
#include <pxr/imaging/hdx/compositor.h>

#include "pxr/imaging/hdAi/nodes/nodes.h"
#include "pxr/imaging/hdAi/renderDelegate.h"

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderPass : public HdRenderPass {
public:
    HDAI_API
    HdAiRenderPass(
        HdAiRenderDelegate* delegate, HdRenderIndex* index,
        const HdRprimCollection& collection);
    HDAI_API
    ~HdAiRenderPass() override;

    bool IsConverged() const { return _isConverged; }

protected:
    HDAI_API
    void _Execute(
        const HdRenderPassStateSharedPtr& renderPassState,
        const TfTokenVector& renderTags) override;

private:
    std::vector<AtRGBA8> _colorBuffer;
    std::vector<float> _depthBuffer;
    HdAiRenderDelegate* _delegate;
    AtNode* _camera = nullptr;
    AtNode* _beautyFilter = nullptr;
    AtNode* _closestFilter = nullptr;
    AtNode* _driver = nullptr;

    HdxCompositor _compositor;

    GfMatrix4d _viewMtx;
    GfMatrix4d _projMtx;

    int _width = 0;
    int _height = 0;

    bool _isConverged = false;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDER_PASS_H
