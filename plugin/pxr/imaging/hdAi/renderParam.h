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
#ifndef HDAI_RENDER_PARAM_H
#define HDAI_RENDER_PARAM_H

#include <pxr/pxr.h>

#include <pxr/imaging/hd/renderDelegate.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderParam final : public HdRenderParam {
public:
    ~HdAiRenderParam() override = default;

    bool Render();
    void Restart();
    void End();
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
