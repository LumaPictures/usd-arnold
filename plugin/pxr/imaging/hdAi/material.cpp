// Copyright (c) 2019 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "pxr/imaging/hdAi/material.h"

#include <pxr/usdImaging/usdImaging/tokens.h>

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
    AtNode* ret = nullptr;
    const auto nodeIt = _nodes.find(nodeName);
    if (nodeIt != _nodes.end()) {
        if (AiNodeEntryGetNameAtString(AiNodeGetNodeEntry(nodeIt->second)) !=
            nodeType) {
            AiNodeDestroy(nodeIt->second);
            _nodes.erase(nodeIt);
        } else {
            ret = nodeIt->second;
        }
    }
    if (ret == nullptr) {
        ret = AiNode(_delegate->GetUniverse(), nodeType);
        if (ret == nullptr) { return nullptr; }
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
