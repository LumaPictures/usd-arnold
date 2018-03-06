#ifndef __USDAI_VOLUMEWRITER_H__
#define __USDAI_VOLUMEWRITER_H__

#include <pxr/pxr.h>

#include <gusd/primWrapper.h>

#include <pxr/usd/usdAi/aiVolume.h>

PXR_NAMESPACE_OPEN_SCOPE

class AiVolumeWrapper : public GusdPrimWrapper {
public:
    static GT_PrimitiveHandle
    defineForWrite(
        const GT_PrimitiveHandle& sourcePrim,
        const UsdStagePtr& stage,
        const SdfPath& path,
        const GusdContext& ctxt);

    AiVolumeWrapper(
        const UsdStagePtr& stage,
        const SdfPath& path,
        bool isOverride = false);

    const char* className() const override;

    void enlargeBounds(UT_BoundingBox boxes[], int nsegments) const override;

    int getMotionSegments() const override;

    int64 getMemoryUsage() const override;

    GT_PrimitiveHandle doSoftCopy() const override;

    const UsdGeomImageable getUsdPrim() const override { return m_volume; }

    bool
    updateFromGTPrim(
        const GT_PrimitiveHandle& sourcePrim,
        const UT_Matrix4D& houXform,
        const GusdContext& ctxt,
        GusdSimpleXformCache& xformCache) override;

    bool isValid() const override { return m_volume; };
private:
    UsdAiVolume m_volume;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif

