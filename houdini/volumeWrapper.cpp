#include "volumeWrapper.h"

#include <gusd/context.h>

PXR_NAMESPACE_OPEN_SCOPE

GT_PrimitiveHandle
AiVolumeWrapper::defineForWrite(
    const GT_PrimitiveHandle& sourcePrim,
    const UsdStagePtr& stage,
    const SdfPath& path,
    const GusdContext& ctxt) {
    return new AiVolumeWrapper(stage, path, ctxt.writeOverlay);
}

AiVolumeWrapper::AiVolumeWrapper(
    const UsdStagePtr& stage,
    const SdfPath& path,
    bool isOverride) {
    if (isOverride) {
        m_volume = UsdAiVolume(stage->OverridePrim(path));
    } else {
        m_volume = UsdAiVolume::Define(stage, path);
    }
}

const char*
AiVolumeWrapper::className() const {
    return "AiVolumeWrapper";
}

void
AiVolumeWrapper::enlargeBounds(UT_BoundingBox boxes[], int nsegments) const {

}

int
AiVolumeWrapper::getMotionSegments() const {
    return 1;
}

int64
AiVolumeWrapper::getMemoryUsage() const {
    return 0;
}

GT_PrimitiveHandle
AiVolumeWrapper::doSoftCopy() const {
    return GT_PrimitiveHandle(new AiVolumeWrapper(*this));
}

PXR_NAMESPACE_CLOSE_SCOPE
