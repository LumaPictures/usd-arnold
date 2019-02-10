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
