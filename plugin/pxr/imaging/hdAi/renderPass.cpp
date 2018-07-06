#include "pxr/imaging/hdAi/renderPass.h"

#include <pxr/imaging/hd/renderPassState.h>

PXR_NAMESPACE_OPEN_SCOPE

HdAiRenderPass::HdAiRenderPass(HdRenderIndex* index, const HdRprimCollection& collection)
    : HdRenderPass(index, collection) {
    _camera = AiNode("persp_camera");
    AiNodeSetPtr(AiUniverseGetOptions(), "camera", _camera);
    AiNodeSetStr(_camera, "name", "HdAiRenderPass_camera");
    _filter = AiNode("gaussian_filter");
    AiNodeSetStr(_filter, "name", "HdAiRenderPass_filter");
}

void
HdAiRenderPass::_Execute(
    const HdRenderPassStateSharedPtr& renderPassState,
    const TfTokenVector& renderTags) {
    if (AiRendering()) {
        AiRenderAbort(AI_BLOCKING);
    }

    AiRender();
}

PXR_NAMESPACE_CLOSE_SCOPE
