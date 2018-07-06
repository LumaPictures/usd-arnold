#ifndef HDAI_RENDERER_PLUGIN_H
#define HDAI_RENDERER_PLUGIN_H

#include <pxr/pxr.h>
#include <pxr/imaging/hdx/rendererPlugin.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRendererPlugin final : public HdxRendererPlugin {
public:
    HdAiRendererPlugin() = default;
    ~HdAiRendererPlugin() override = default;

    // This class does not support copying.
    HdAiRendererPlugin(const HdAiRendererPlugin&) = delete;
    HdAiRendererPlugin &operator =(const HdAiRendererPlugin&) = delete;

    HdRenderDelegate *CreateRenderDelegate() override;
    void DeleteRenderDelegate(
        HdRenderDelegate *renderDelegate) override;
    bool IsSupported() const override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDERER_PLUGIN_H
