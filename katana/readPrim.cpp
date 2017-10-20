#include "readPrim.h"

#include <pxr/usd/usdAi/aiShapeAPI.h>
#include <pxr/usd/usdAi/aiMaterialAPI.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/connectableAPI.h>

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

    // It's hard to decide the exact frequency of inserts
    // and reads, but most likely it's the same magnitude.
    // We could also try a vector here.
    std::set<std::string> processedMaterials;

    // We can't use auto here, otherwise the lambda won't be able to capture itself.
    std::function<void(const UsdPrim&)> traverseShader
        = [&](const UsdPrim& shader) {
        // TODO: we can also use getInputs from the new API.
        const auto shadingNodeHandle = PxrUsdKatanaUtils::GenerateShadingNodeHandle(shader);
        if (processedMaterials.find(shadingNodeHandle) != processedMaterials.end()) {
            return;
        }
        processedMaterials.insert(shadingNodeHandle);
        static const std::string baseAttr("material.nodes.");
        std::stringstream ss; ss << baseAttr << shadingNodeHandle;
        if (!interface.getOutputAttr(ss.str()).isValid()) { return; }
        FnKat::GroupBuilder builder;
        for (const auto& relationship: shader.GetRelationships()) {
            static const std::string connectedSourceFor("connectedSourceFor:");
            const auto relationshipName = relationship.GetName().GetString();
            if (relationshipName.compare(0, connectedSourceFor.length(), connectedSourceFor) != 0) { continue; }

            auto paramName = relationshipName.substr(connectedSourceFor.length());
            auto colonPos = paramName.find(':');
            if (colonPos != paramName.npos) { // either array elem or component
                auto comp = paramName.substr(colonPos + 1);
                paramName = paramName.substr(0, colonPos);
                if (comp.length() == 0) { continue; } // Just to make sure it's not a malformed
                // variable, like one that ends with a :
                if (comp[0] == 'i') { // array connection
                    paramName += ":" + comp.substr(1);
                } else {
                    paramName += "." + comp;
                }
            } else {
                continue; // The existing code already handles this!
            }

            static __thread SdfPathVector targets;
            targets.clear();
            relationship.GetTargets(&targets);
            if (targets.size() != 1) { continue; }

            const auto& target = targets.front();
            auto targetName = target.GetName();
            static const std::string outputs("outputs:");
            if (targetName.compare(0, outputs.length(), outputs) != 0) { continue; }
            targetName = targetName.substr(outputs.length());
            if (targetName != "out") { // component connection
                targetName = "out." + targetName;
            }
            std::stringstream targetSS; targetSS << targetName << '@';
            targetSS << target.GetParentPath().GetName();
            builder.set(paramName, FnKat::StringAttribute(targetSS.str()));

            // TODO: we might traverse things twice because of this.
            // Also, infinite recursion, beware!
            mapRelations(relationship, traverseShader);
        }

        static constexpr auto _maxSplitCount = 3;
        using param_split_t = std::array<std::string, _maxSplitCount>;
        auto splitParamName = [] (const std::string& name,
                                  param_split_t& out) -> size_t {
            size_t outputCount = 0;
            size_t currentIndex = 0;
            while (outputCount < _maxSplitCount) {
                size_t colonPos = name.find(':', currentIndex);
                out[outputCount++] = name.substr(currentIndex, colonPos - currentIndex);
                if (colonPos == name.npos) { break; }
                currentIndex = colonPos + 1;
            }
            return outputCount;
        };

        // TODO: Check for more optimal data storage options
        std::set<std::string> fullConnections;
        std::set<std::string> partialConnections;
        // Per array connections already work properly with the new API.
        // It seems we can't do full connections and component connections at the same
        // time. To support both combined (full connections and partial connections
        // we have to collect which full connections are explicitly set by the user,
        // and which partial ones. After that we need to manually connect the rest of the
        // component connections not covered by the user.
        UsdShadeConnectableAPI connectableAPI(shader);
        if (connectableAPI) {
            for (const auto& input: connectableAPI.GetInputs()) {
                if (input.HasConnectedSource()) {
                    const auto inParamName = input.GetBaseName().GetString();
                    SdfPathVector sourcePaths;
                    input.GetRawConnectedSourcePaths(&sourcePaths);
                    if (sourcePaths.size() == 0) { continue; }
                    // We are only checking for the first connection
                    const auto& sourceParamPath = sourcePaths[0];
                    const auto sourcePath = sourceParamPath.GetPrimPath();
                    const auto sourcePrim = stage->GetPrimAtPath(sourcePath);
                    if (sourcePrim.IsValid()) {
                        traverseShader(shader);
                    }
                    static __thread param_split_t _paramSplit;
                    const auto splitCount = splitParamName(inParamName, _paramSplit);
                    if (splitCount == 0) {
                        // Something bad have happened, move on.
                        continue;
                    } else if (splitCount == 1) {
                        // Full param connection, usdKatana already handles this
                        // add it to the full list and move on.
                        fullConnections.insert(inParamName);
                        continue;
                    } else if (splitCount == _maxSplitCount) {
                        // Connection to Array elements, no idea how to handle this.
                        // Yet.
                        continue;
                    }

                    // Connections to array elements are already handled.
                    // Or we are having an invalid string.
                    if (_paramSplit[1].empty() || _paramSplit[1].front() == 'i') {
                        continue;
                    }

                    const std::string paramName = _paramSplit[0] + "." + _paramSplit[1];

                    const auto sourceParam = sourceParamPath.GetName();
                    static __thread param_split_t _sourceParamSplit;
                    const auto sourceSplitCount = splitParamName(sourceParam, _sourceParamSplit);
                    if (sourceSplitCount != 2) { continue; } // we only support component connections for now
                    if (_sourceParamSplit[1].empty() ||
                        _sourceParamSplit[1].front() == 'i') {
                        continue;
                    }
                    partialConnections.insert(_paramSplit[0]);
                    const auto sourceParamAndComponentName =
                    _sourceParamSplit[1] + "@" + sourcePath.GetName();
                    builder.set(paramName,
                                FnKat::StringAttribute(sourceParamAndComponentName));
                }
            }
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
