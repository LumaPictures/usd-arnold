#include "pxr/pxr.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usdGeom/xform.h"

#include "pxr/usd/usdAi/aiShapeAPI.h"

PXR_NAMESPACE_USING_DIRECTIVE

constexpr auto framesPerSecond = 24.0;
const SdfPath xformPath("/xform");

void runTest() {
    auto stage = UsdStage::CreateInMemory("test.usda");
    stage->SetFramesPerSecond(framesPerSecond); // Do we need to set this up?

    auto xform = UsdGeomXform::Define(stage, xformPath);
    TF_VERIFY(xform, "Failed to create xofrm at %s", xformPath.GetText());
}

int main() {
    runTest();
}
