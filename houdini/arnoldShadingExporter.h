#ifndef __USDAI_ARNOLDSHADINGEXPORTER_H__
#define __USDAI_ARNOLDSHADINGEXPORTER_H__

#include <pxr/pxr.h>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/sdf/path.h>

#include <gusd/shadingModeRegistry.h>

#include <OP/OP_Node.h>

#include <string>

PXR_NAMESPACE_OPEN_SCOPE

void exportArnoldShaders(
    OP_Node* opNode,
    const UsdStagePtr& stage,
    const SdfPath& looksPath,
    const GusdShadingModeRegistry::HouMaterialMap& houMaterialMap,
    const std::string& shaderOutDir);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // __USDAI_ARNOLDSHADINGEXPORTER_H__
