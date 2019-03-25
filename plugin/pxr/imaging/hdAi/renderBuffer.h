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
#ifndef HDAI_RENDER_BUFFER_H
#define HDAI_RENDER_BUFFER_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/renderBuffer.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderBuffer : public HdRenderBuffer {
public:
    HDAI_API
    HdAiRenderBuffer(const SdfPath& id);
    HDAI_API
    ~HdAiRenderBuffer() override = default;

    HDAI_API
    bool Allocate(
        const GfVec3i& dimensions, HdFormat format, bool multiSampled) override;

    HDAI_API
    unsigned int GetWidth() const override;
    HDAI_API
    unsigned int GetHeight() const override;
    HDAI_API
    unsigned int GetDepth() const override;
    HDAI_API
    HdFormat GetFormat() const override;
    HDAI_API
    bool IsMultiSampled() const override;
    HDAI_API
    uint8_t* Map() override;
    HDAI_API
    void Unmap() override;
    HDAI_API
    bool IsMapped() const override;
    HDAI_API
    void Resolve() override;
    HDAI_API
    bool IsConverged() const override;

protected:
    HDAI_API
    void _Deallocate() override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDER_BUFFER_H
