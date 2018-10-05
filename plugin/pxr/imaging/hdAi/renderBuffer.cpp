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
#include "pxr/imaging/hdAi/renderBuffer.h"

#include "renderBuffer.h"

PXR_NAMESPACE_OPEN_SCOPE

HdAiRenderBuffer::HdAiRenderBuffer(const SdfPath& id) : HdRenderBuffer(id) {}

bool HdAiRenderBuffer::Allocate(
    const GfVec3i& dimensions, HdFormat format, bool multiSampled) {
    return false;
}

unsigned int HdAiRenderBuffer::GetWidth() const { return 0; }

unsigned int HdAiRenderBuffer::GetHeight() const { return 0; }

unsigned int HdAiRenderBuffer::GetDepth() const { return 0; }

HdFormat HdAiRenderBuffer::GetFormat() const { return HdFormatUNorm8; }

bool HdAiRenderBuffer::IsMultiSampled() const { return false; }

uint8_t* HdAiRenderBuffer::Map() { return nullptr; }

void HdAiRenderBuffer::Unmap() {}

bool HdAiRenderBuffer::IsMapped() const { return false; }

void HdAiRenderBuffer::Resolve() {}

bool HdAiRenderBuffer::IsConverged() const { return false; }

void HdAiRenderBuffer::_Deallocate() {}

PXR_NAMESPACE_CLOSE_SCOPE
