// Copyright 2019 Luma Pictures
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "pxr/imaging/hdAi/material.h"

#include <pxr/usdImaging/usdImaging/tokens.h>

#include "pxr/imaging/hdAi/debugCodes.h"
#include "pxr/imaging/hdAi/utils.h"

PXR_NAMESPACE_OPEN_SCOPE

namespace {
const AtString nameStr("name");
}

HdAiMaterial::HdAiMaterial(HdAiRenderDelegate* delegate, const SdfPath& id)
    : HdMaterial(id), _delegate(delegate) {
    _surface = _delegate->GetFallbackShader();
}

HdAiMaterial::~HdAiMaterial() {
    for (auto& node : _nodes) { AiNodeDestroy(node.second); }
}

void HdAiMaterial::Sync(
    HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits) {
    auto* param = reinterpret_cast<HdAiRenderParam*>(renderParam);
    const auto id = GetId();
    if ((*dirtyBits & HdMaterial::DirtyResource) && !id.IsEmpty()) {
        param->End();
        auto value = sceneDelegate->GetMaterialResource(GetId());
        if (value.IsHolding<HdMaterialNetworkMap>()) {
            const auto& map = value.UncheckedGet<HdMaterialNetworkMap>();
            const auto* network =
                TfMapLookupPtr(map.map, UsdImagingTokens->bxdf);
            if (network != nullptr) {
                auto* entry = ReadMaterialNetwork(*network);
                _surface =
                    entry == nullptr ? _delegate->GetFallbackShader() : entry;
            }
        }
    }
    *dirtyBits = HdMaterial::Clean;
}

HdDirtyBits HdAiMaterial::GetInitialDirtyBitsMask() const {
    return HdMaterial::DirtyResource;
}

void HdAiMaterial::Reload() {}

AtNode* HdAiMaterial::GetSurfaceShader() const { return _surface; }

AtNode* HdAiMaterial::GetDisplacementShader() const { return _displacement; }

AtNode* HdAiMaterial::ReadMaterialNetwork(const HdMaterialNetwork& network) {
    TF_DEBUG(HDAI_MATERIAL)
        .Msg(
            "HdAiMaterial::ReadMaterialNetwork - %s - num nodes: %lu\n",
            GetId().GetText(), network.nodes.size());
    if (network.nodes.empty()) { return nullptr; }

    std::vector<AtNode*> nodes;
    nodes.reserve(network.nodes.size());
    for (const auto& node : network.nodes) {
        auto* n = ReadMaterial(node);
        if (n != nullptr) { nodes.push_back(n); }
    }

    // Currently USD can't describe partial connections, so we just do a full
    // link.
    for (const auto& relationship : network.relationships) {
        auto* inputNode = FindMaterial(relationship.inputId);
        if (inputNode == nullptr) { continue; }
        std::remove(nodes.begin(), nodes.end(), inputNode);
        auto* outputNode = FindMaterial(relationship.outputId);
        if (outputNode == nullptr) { continue; }
        AiNodeLink(inputNode, relationship.outputName.GetText(), outputNode);
    }

    return nodes.empty() ? nullptr : nodes.front();
}

AtNode* HdAiMaterial::ReadMaterial(const HdMaterialNode& material) {
    const auto nodeName = GetLocalNodeName(material.path);
    const auto* nodeTypeStr = material.identifier.GetText();
    const AtString nodeType(
        strncmp(nodeTypeStr, "ai:", 3) == 0 ? nodeTypeStr + 3 : nodeTypeStr);

    TF_DEBUG(HDAI_MATERIAL)
        .Msg(
            "HdAiMaterial::ReadMaterial - node %s - type %s\n",
            nodeName.c_str(), nodeType.c_str());
    AtNode* ret = nullptr;
    const auto nodeIt = _nodes.find(nodeName);
    if (nodeIt != _nodes.end()) {
        if (AiNodeEntryGetNameAtString(AiNodeGetNodeEntry(nodeIt->second)) !=
            nodeType) {
            TF_DEBUG(HDAI_MATERIAL)
                .Msg(
                    "  existing node found, but type mismatch - deleting old "
                    "node\n");
            AiNodeDestroy(nodeIt->second);
            _nodes.erase(nodeIt);
        } else {
            TF_DEBUG(HDAI_MATERIAL).Msg("  existing node found - using it\n");
            ret = nodeIt->second;
        }
    }
    if (ret == nullptr) {
        ret = AiNode(_delegate->GetUniverse(), nodeType);
        if (ret == nullptr) {
            TF_DEBUG(HDAI_MATERIAL)
                .Msg(
                    "  unable to create node of type %s - aborting\n",
                    nodeType.c_str());
            return nullptr;
        }
        TF_DEBUG(HDAI_MATERIAL)
            .Msg("  created node of type %s\n", nodeType.c_str());
        AiNodeSetStr(ret, nameStr, nodeName);
        _nodes.emplace(nodeName, ret);
    }

    const auto* nentry = AiNodeGetNodeEntry(ret);
    for (const auto& param : material.parameters) {
        const auto& paramName = param.first;
        const auto* pentry =
            AiNodeEntryLookUpParameter(nentry, AtString(paramName.GetText()));
        if (pentry == nullptr) { continue; }
        HdAiSetParameter(ret, pentry, param.second);
    }
    return ret;
}

AtNode* HdAiMaterial::FindMaterial(const SdfPath& path) const {
    const auto nodeIt = _nodes.find(GetLocalNodeName(path));
    return nodeIt == _nodes.end() ? nullptr : nodeIt->second;
}

AtString HdAiMaterial::GetLocalNodeName(const SdfPath& path) const {
    const auto* pp = path.GetText();
    if (pp == nullptr || pp[0] == '\0') { return AtString(path.GetText()); }
    const auto p = GetId().AppendPath(SdfPath(TfToken(pp + 1)));
    return AtString(p.GetText());
}

PXR_NAMESPACE_CLOSE_SCOPE
