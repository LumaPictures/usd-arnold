#include "pxr/pxr.h"
#include "pxr/base/tf/pyModule.h"

PXR_NAMESPACE_USING_DIRECTIVE

TF_WRAP_MODULE
{
    TF_WRAP(UsdAiMaterialAPI);
    TF_WRAP(UsdAiNodeAPI);
    TF_WRAP(UsdAiProcedural);
    TF_WRAP(UsdAiShader);
    TF_WRAP(UsdAiVolume);
}
