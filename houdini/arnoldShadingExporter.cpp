#include <gusd/shadingModeRegistry.h>

PXR_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION_WITH_TAG(GusdShadingModeRegistry, rib) {
    GusdShadingModeRegistry::getInstance().registerExporter(
        "arnold", "Arnold", [](
            OP_Node* opNode,
            const UsdStagePtr& stage,
            const SdfPath& looksPath,
            const GusdShadingModeRegistry::HouMaterialMap& houMaterialMap,
            const std::string& shaderOutDir) {
            for (const auto& assignment: houMaterialMap) {
                std::cerr << "Trying to assign " << assignment.first << std::endl;
                std::cerr << "To prims " << std::endl;
                for (const auto& primPath: assignment.second) {
                    std::cerr << primPath << std::endl;
                }
            }
        });
}

PXR_NAMESPACE_CLOSE_SCOPE