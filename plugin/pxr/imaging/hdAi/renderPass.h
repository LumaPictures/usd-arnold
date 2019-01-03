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
