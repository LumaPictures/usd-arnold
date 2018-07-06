#ifndef HDAI_RENDER_PASS_H
#define HDAI_RENDER_PASS_H

#include <pxr/pxr.h>

#include <pxr/imaging/hd/renderPass.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderPass : public HdRenderPass {
public:
    HdAiRenderPass(HdRenderIndex* index, const HdRprimCollection& collection);
    ~HdAiRenderPass() override = default;

protected:
    void _Execute(
        const HdRenderPassStateSharedPtr& renderPassState,
        const TfTokenVector& renderTags);
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDER_PASS_H
