// Copyright (c) 2018 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef HDAI_RENDER_BUFFER_H
#define HDAI_RENDER_BUFFER_H

#include <pxr/pxr.h>

#include <pxr/imaging/hd/renderBuffer.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderBuffer : public HdRenderBuffer {
public:
    HdAiRenderBuffer(const SdfPath& id);
    ~HdAiRenderBuffer() override = default;

    bool Allocate(
        const GfVec3i& dimensions, HdFormat format, bool multiSampled) override;

    unsigned int GetWidth() const override;
    unsigned int GetHeight() const override;
    unsigned int GetDepth() const override;
    HdFormat GetFormat() const override;
    bool IsMultiSampled() const override;
    uint8_t* Map() override;
    void Unmap() override;
    bool IsMapped() const override;
    void Resolve() override;

    bool IsConverged() const override;

private:
    void _Deallocate() override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDER_BUFFER_H
