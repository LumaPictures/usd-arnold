#include "readPrim.h"

#include <pxr/usd/usdAi/aiShapeAPI.h>
#include <pxr/usd/usdAi/aiMaterialAPI.h>
#include <pxr/usd/usdShade/material.h>

#include <usdKatana/attrMap.h>
#include <usdKatana/utils.h>

#include "arnoldHelpers.h"

PXR_NAMESPACE_OPEN_SCOPE

void readPrimLocation(
    FnKat::GeolibCookInterface& interface,
    FnKat::GroupAttribute opArgs,
    PxrUsdKatanaUsdInPrivateData* privateData) {
    // privateData can be a nullptr!
    if (privateData == nullptr) { return; }
    const auto prim = privateData->GetUsdPrim();
    if (!prim.IsValid()) { return; }

    // Based on PxrUsdKatanaAttrMap::toInterface
    auto updateOrCreateAttr = [&interface] (const std::string& attrName, const FnKat::Attribute& attr) {
        if (!attr.isValid()) { return;  }

        if (attr.getType() == kFnKatAttributeTypeGroup) {
            FnAttribute::GroupAttribute existingAttr = interface.getOutputAttr(attrName);
            if (existingAttr.isValid()) {
                interface.setAttr(attrName
                    , FnAttribute::GroupBuilder()
                                      .update(existingAttr)
                                      .deepUpdate(attr)
                                      .build());
            } else {
                interface.setAttr(attrName, attr);
            }
        } else {
            interface.setAttr(attrName, attr);
        }
    };

    static const std::string statementsName("arnoldStatements");
    updateOrCreateAttr(statementsName, GetArnoldStatementsGroup(prim));

    auto stage = prim.GetStage();
    if (stage == nullptr) { return; }

    auto mapRelations = [&stage] (const UsdRelationship& relationship
        , std::function<void(const UsdPrim&)> fn) {
        static __thread SdfPathVector targets; targets.clear();
        relationship.GetTargets(&targets);
        for (const auto& target: targets) {
            const auto shader = stage->GetPrimAtPath(target);
            if (shader.IsValid()) { fn(shader); }
        }
    };

    // We can't use auto here, otherwise the lambda won't be able to capture itself.
    std::function<void(const UsdPrim&)> traverseShader
        = [&](const UsdPrim& shader) {
        // TODO: we can also use getInputs from the new API.
        const auto shadingNodeHandle = PxrUsdKatanaUtils::GenerateShadingNodeHandle(shader);
        static const std::string baseAttr("material.nodes.");
        std::stringstream ss; ss << baseAttr << shadingNodeHandle;
        if (!interface.getOutputAttr(ss.str()).isValid()) { return; }
        const auto relationships = shader.GetRelationships();
        FnKat::GroupBuilder builder;
        for (const auto& relationship: relationships) {
            static const std::string connectedSourceFor("connectedSourceFor:");
            const auto relationshipName = relationship.GetName().GetString();
            if (relationshipName.compare(0, connectedSourceFor.length(), connectedSourceFor) != 0) { continue; }

            static __thread SdfPathVector targets;
            targets.clear();
            relationship.GetTargets(&targets);
            if (targets.size() != 1) { continue; }

            auto paramName = relationshipName.substr(connectedSourceFor.length());
            auto colonPos = paramName.find(':');
            if (colonPos != paramName.npos) { // either array elem or component
                auto comp = paramName.substr(colonPos + 1);
                paramName = paramName.substr(0, colonPos + 1);
                if (comp.length() == 0) { continue; }
                if (comp[0] == 'i') { // array connection
                    paramName += comp.substr(1);
                }
            }

            const auto& target = targets.front();
            const auto& targetName = target.GetName();
            colonPos = targetName.find(':');
            if (colonPos == targetName.npos) { continue; }
            std::stringstream targetSS; targetSS << targetName.substr(colonPos + 1) << '@';
            targetSS << target.GetParentPath().GetName();
            builder.set(paramName, FnKat::StringAttribute(targetSS.str()));

            // TODO: we shouldn't map this twice.
            mapRelations(relationship, traverseShader);
        }

        FnKat::Attribute connections = builder.isValid() ? builder.build() : FnKat::Attribute();
        ss << ".connections";
        updateOrCreateAttr(ss.str(), connections);
    };

    // We are handling connections here, because usd-arnold stores the connections in it's own way.
    // So we check for the materials connected to the node.
    const UsdShadeMaterial material(prim);
    if (material) {
        UsdAiMaterialAPI aiMaterialAPI(prim);
        mapRelations(aiMaterialAPI.GetSurfaceRel(), traverseShader);
        mapRelations(aiMaterialAPI.GetDisplacementRel(), traverseShader);
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
