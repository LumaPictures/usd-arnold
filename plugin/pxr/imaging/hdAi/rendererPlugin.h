#ifndef HDAI_RENDERER_PLUGIN_H
#define HDAI_RENDERER_PLUGIN_H

#include "pxr/pxr.h"
#include "pxr/imaging/hdx/rendererPlugin.h"

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRendererPlugin final : public HdxRendererPlugin {
public:
    HdAiRendererPlugin() = default;
    virtual ~HdAiRendererPlugin() = default;

    virtual HdRenderDelegate *CreateRenderDelegate() override;
    virtual void DeleteRenderDelegate(
        HdRenderDelegate *renderDelegate) override;
    virtual bool IsSupported() const override;
private:
    // This class does not support copying.
    HdAiRendererPlugin(const HdAiRendererPlugin&) = delete;
    HdAiRendererPlugin &operator =(const HdAiRendererPlugin&) = delete;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDERER_PLUGIN_H
