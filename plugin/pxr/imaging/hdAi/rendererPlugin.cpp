#include "rendererPlugin.h"

#include <pxr/imaging/hdx/rendererPluginRegistry.h>

#include "renderDelegate.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the Ai plugin with the renderer plugin system.
TF_REGISTRY_FUNCTION(TfType)
{
    HdxRendererPluginRegistry::Define<HdAiRendererPlugin>();
}

HdRenderDelegate*
HdAiRendererPlugin::CreateRenderDelegate()
{
    // return new HdAiRenderDelegate();
    return nullptr;
}

void
HdAiRendererPlugin::DeleteRenderDelegate(HdRenderDelegate *renderDelegate)
{
    delete renderDelegate;
}

bool
HdAiRendererPlugin::IsSupported() const
{
    return true;
}

PXR_NAMESPACE_CLOSE_SCOPE
