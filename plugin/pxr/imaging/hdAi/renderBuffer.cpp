#include "pxr/imaging/hdAi/renderBuffer.h"
#include "renderBuffer.h"


PXR_NAMESPACE_OPEN_SCOPE

HdAiRenderBuffer::HdAiRenderBuffer(const SdfPath& id)
    : HdRenderBuffer(id) {
}

bool HdAiRenderBuffer::Allocate(const GfVec3i& dimensions, HdFormat format, bool multiSampled) {
    return false;
}

unsigned int HdAiRenderBuffer::GetWidth() const {
    return 0;
}

unsigned int HdAiRenderBuffer::GetHeight() const {
    return 0;
}

unsigned int HdAiRenderBuffer::GetDepth() const {
    return 0;
}

HdFormat HdAiRenderBuffer::GetFormat() const {
    return HdFormatUNorm8;
}

bool HdAiRenderBuffer::IsMultiSampled() const {
    return false;
}

uint8_t* HdAiRenderBuffer::Map() {
    return nullptr;
}

void HdAiRenderBuffer::Unmap() {

}

bool HdAiRenderBuffer::IsMapped() const {
    return false;
}

void HdAiRenderBuffer::Resolve() {

}

bool HdAiRenderBuffer::IsConverged() const {
    return false;
}

void HdAiRenderBuffer::_Deallocate() {

}

PXR_NAMESPACE_CLOSE_SCOPE
