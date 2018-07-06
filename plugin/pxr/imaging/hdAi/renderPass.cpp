#include "pxr/imaging/hdAi/renderPass.h"

PXR_NAMESPACE_OPEN_SCOPE

HdAiRenderPass::HdAiRenderPass(HdRenderIndex* index, const HdRprimCollection& collection)
    : HdRenderPass(index, collection) {

}

void
HdAiRenderPass::_Execute(
    const HdRenderPassStateSharedPtr& renderPassState,
    const TfTokenVector& renderTags) {

}

PXR_NAMESPACE_CLOSE_SCOPE
