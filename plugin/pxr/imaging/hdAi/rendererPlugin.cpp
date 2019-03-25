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
#include "pxr/imaging/hdAi/rendererPlugin.h"

#include <pxr/imaging/hdx/rendererPluginRegistry.h>

#include "pxr/imaging/hdAi/renderDelegate.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the Ai plugin with the renderer plugin system.
TF_REGISTRY_FUNCTION(TfType) {
    HdxRendererPluginRegistry::Define<HdAiRendererPlugin>();
}

HdRenderDelegate* HdAiRendererPlugin::CreateRenderDelegate() {
    return new HdAiRenderDelegate();
}

void HdAiRendererPlugin::DeleteRenderDelegate(
    HdRenderDelegate* renderDelegate) {
    delete renderDelegate;
}

bool HdAiRendererPlugin::IsSupported() const { return true; }

PXR_NAMESPACE_CLOSE_SCOPE
