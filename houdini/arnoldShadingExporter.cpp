#include <gusd/shadingModeRegistry.h>

#include <VOP/VOP_Node.h>
#include <SHOP/SHOP_Node.h>

#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>

#include <pxr/usd/usdAi/aiMaterialAPI.h>
#include <pxr/usd/usdAi/aiShader.h>

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

namespace {
    SdfPath
    getPathNoFirstSlashFromOp(OP_Node* op) {
        const auto* pathStr = op->getFullPath().c_str();
        // We got an invalid path name. Not sure if this is even possible
        // We need to skip the first / from the path or else USD won't be able to append it.
        return (pathStr == nullptr || pathStr[0] != '/') ? SdfPath() : SdfPath(pathStr + 1);
    }

    SdfPath
    exportNode(const UsdStagePtr& stage, const SdfPath& looksPath, VOP_Node* vop) {
        const auto vopTypeName = vop->getOperator()->getName();
        const auto inShaderPath = getPathNoFirstSlashFromOp(vop);
        if (inShaderPath.IsEmpty()) { return SdfPath(); }
        const auto outShaderPath = looksPath.AppendPath(inShaderPath);
        // We already exported the shader.
        if (stage->GetPrimAtPath(outShaderPath).IsValid()) { return outShaderPath; }
        static constexpr auto aiShaderPrefix = "arnold::";
        static constexpr auto aiShaderPrefixLength = strlen(aiShaderPrefix);
        // We only export arnold nodes, and they should start with arnold::
        if (!vopTypeName.startsWith(aiShaderPrefix)) { return SdfPath(); }
        // Yes! I know I should use substring, but that api on UT_String is so ugly.
        const std::string aiTypeName(vopTypeName.c_str() + aiShaderPrefixLength);
        const auto aiShader = UsdAiShader::Define(stage, outShaderPath);
        aiShader.CreateIdAttr().Set(TfToken(aiTypeName));
        return outShaderPath;
    }

    OP_Node*
    findFirstChildrenOfType(OP_Node* op, const char* type) {
        const auto nchildren = op->getNchildren();
        for (auto cid = decltype(nchildren){0}; cid < nchildren; ++cid) {
            auto* ch = op->getChild(cid);
            if (ch->getOperator()->getName() == "arnold_material") {
                return ch;
            }
        }
        return nullptr;
    }
}

TF_REGISTRY_FUNCTION_WITH_TAG(GusdShadingModeRegistry, rib) {
    GusdShadingModeRegistry::getInstance().registerExporter(
        "arnold", "Arnold", [](
            OP_Node* opNode,
            const UsdStagePtr& stage,
            const SdfPath& looksPath,
            const GusdShadingModeRegistry::HouMaterialMap& houMaterialMap,
            const std::string& shaderOutDir) {

            for (const auto& assignment: houMaterialMap) {
                // Initially we only care about assigned shops.
                auto* shop = opNode->findSHOPNode(assignment.first.c_str());
                // We only support arnold_vopnets.
                if (shop->getOperator()->getName() != "arnold_vopnet") { continue; }

                const auto shopPath = getPathNoFirstSlashFromOp(shop);
                if (shopPath.IsEmpty()) { continue; }
                const SdfPath materialPath = looksPath.AppendPath(shopPath);

                auto alreadyExported = false;
                if (stage->GetPrimAtPath(materialPath).IsValid()) {
                    alreadyExported = true;
                }
                auto shadeMaterial = UsdShadeMaterial::Define(stage, materialPath);

                // We are using the new material binding API, the old is deprecated.
                for (const auto& primToBind: assignment.second) {
                    UsdShadeMaterialBindingAPI(stage->GetPrimAtPath(primToBind)).Bind(shadeMaterial);
                }

                // We skip export if it's already exported by somebody else.
                if (alreadyExported) { continue; }

                // We have to find the output node, HtoA simply looks for the first
                // vop node with the type of arnold_material.
                static constexpr auto arnoldMaterialTypeName = "arnold_material";
                auto* possibleArnoldMaterial = findFirstChildrenOfType(shop, arnoldMaterialTypeName);
                auto* vop = possibleArnoldMaterial == nullptr ? nullptr : possibleArnoldMaterial->castToVOPNode();

                if (vop == nullptr) { continue; }

                UsdAiMaterialAPI aiMaterialAPI(shadeMaterial.GetPrim());
                static const std::vector<std::pair<const char*,
                    decltype(&UsdAiMaterialAPI::CreateSurfaceRel)>> materialParams = {
                    {"surface", &UsdAiMaterialAPI::CreateSurfaceRel},
                    {"displacement", &UsdAiMaterialAPI::CreateDisplacementRel},
                    // {"volume", &UsdAiMaterialAPI::CreateVolumeRel} TODO: need volume relationship
                };
                for (const auto& materialParam: materialParams) {
                    const auto idx = vop->getInputFromName(materialParam.first);
                    if (!vop->isConnected(idx, true)) { continue; }
                    auto* inputVOP = vop->findSimpleInput(idx);
                    auto inputPath = exportNode(stage, looksPath, inputVOP);
                    if (!inputPath.IsEmpty()) {
                        ((aiMaterialAPI).*(materialParam.second))().AddTarget(inputPath);
                    }
                }
            }
        });
}

PXR_NAMESPACE_CLOSE_SCOPE
