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
#ifndef HDAI_RENDERER_PLUGIN_H
#define HDAI_RENDERER_PLUGIN_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hdx/rendererPlugin.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRendererPlugin final : public HdxRendererPlugin {
public:
    HDAI_API
    HdAiRendererPlugin() = default;
    HDAI_API
    ~HdAiRendererPlugin() override = default;

    // This class does not support copying.
    HdAiRendererPlugin(const HdAiRendererPlugin &) = delete;
    HdAiRendererPlugin &operator=(const HdAiRendererPlugin &) = delete;

    HDAI_API
    HdRenderDelegate *CreateRenderDelegate() override;
    HDAI_API
    void DeleteRenderDelegate(HdRenderDelegate *renderDelegate) override;
    HDAI_API
    bool IsSupported() const override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDERER_PLUGIN_H
