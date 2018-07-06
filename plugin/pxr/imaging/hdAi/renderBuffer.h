#ifndef HDAI_RENDER_BUFFER_H
#define HDAI_RENDER_BUFFER_H

#include <pxr/pxr.h>

#include <pxr/imaging/hd/renderBuffer.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderBuffer : public HdRenderBuffer {
public:
    HdAiRenderBuffer(const SdfPath& id);
    ~HdAiRenderBuffer() override = default;

    bool
    Allocate(const GfVec3i& dimensions, HdFormat format, bool multiSampled) override;

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
