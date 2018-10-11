// Copyright (c) 2018 Luma Pictures . All rights reserved.
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

#include <pxr/base/gf/vec2f.h>

#include <pxr/usdImaging/usdImaging/tokens.h>

PXR_NAMESPACE_OPEN_SCOPE

namespace {
const AtString nameName("name");
}

HdAiMaterial::HdAiMaterial(HdAiRenderDelegate* delegate, const SdfPath& id)
    : HdMaterial(id), _delegate(delegate) {
    _surface = _delegate->GetFallbackShader();
}

void HdAiMaterial::Sync(
    HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits) {
    TF_UNUSED(renderParam);
    const auto id = GetId();
    if ((*dirtyBits & HdMaterial::DirtyResource) && !id.IsEmpty()) {
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

void HdAiMaterial::SetParameter(
    AtNode* node, const AtParamEntry* pentry, const VtValue& value) {
    const auto paramName = AiParamGetName(pentry);
    switch (AiParamGetType(pentry)) {
        case AI_TYPE_BYTE:
            if (value.IsHolding<int>()) {
                AiNodeSetByte(
                    node, paramName,
                    static_cast<uint8_t>(value.UncheckedGet<int>()));
            }
            break;
        case AI_TYPE_INT:
            if (value.IsHolding<int>()) {
                AiNodeSetInt(node, paramName, value.UncheckedGet<int>());
            }
            break;
        case AI_TYPE_UINT:
        case AI_TYPE_USHORT:
            if (value.IsHolding<unsigned int>()) {
                AiNodeSetUInt(
                    node, paramName, value.UncheckedGet<unsigned int>());
            }
            break;
        case AI_TYPE_BOOLEAN:
            if (value.IsHolding<bool>()) {
                AiNodeSetBool(node, paramName, value.UncheckedGet<bool>());
            }
            break;
        case AI_TYPE_FLOAT:
        case AI_TYPE_HALF:
            if (value.IsHolding<float>()) {
                AiNodeSetFlt(node, paramName, value.UncheckedGet<float>());
            }
            break;
        case AI_TYPE_RGB:
            if (value.IsHolding<GfVec3f>()) {
                const auto& v = value.UncheckedGet<GfVec3f>();
                AiNodeSetRGB(node, paramName, v[0], v[1], v[2]);
            }
            break;
        case AI_TYPE_RGBA:
            if (value.IsHolding<GfVec3f>()) {
                const auto& v = value.UncheckedGet<GfVec4f>();
                AiNodeSetRGBA(node, paramName, v[0], v[1], v[2], v[3]);
            }
            break;
        case AI_TYPE_VECTOR:
            if (value.IsHolding<GfVec3f>()) {
                const auto& v = value.UncheckedGet<GfVec3f>();
                AiNodeSetVec(node, paramName, v[0], v[1], v[2]);
            }
            break;
        case AI_TYPE_VECTOR2:
            if (value.IsHolding<GfVec2f>()) {
                const auto& v = value.UncheckedGet<GfVec2f>();
                AiNodeSetVec2(node, paramName, v[0], v[1]);
            }
            break;
        case AI_TYPE_STRING:
            if (value.IsHolding<TfToken>()) {
                AiNodeSetStr(
                    node, paramName, value.UncheckedGet<TfToken>().GetText());
            }
            break;
        case AI_TYPE_POINTER:
        case AI_TYPE_NODE:
            break; // Should be in the relationships list.
        case AI_TYPE_MATRIX:
            break; // TODO
        case AI_TYPE_ENUM:
            break; // TODO
        case AI_TYPE_CLOSURE:
            break; // Should be in the relationships list.
        default:
            AiMsgError(
                "Unsupported parameter %s.%s", AiNodeGetName(node),
                AiParamGetName(pentry).c_str());
    }
}

AtNode* HdAiMaterial::ReadMaterialNetwork(const HdMaterialNetwork& network) {
    if (network.nodes.empty()) { return nullptr; }

    auto it = network.nodes.cbegin();
    auto* ret = ReadMaterial(*(it++));
    for (const auto end = network.nodes.cend(); it != end; ++it) {
        ReadMaterial(*it);
    }

    // TODO: Build connections.

    return ret;
}

AtNode* HdAiMaterial::ReadMaterial(const HdMaterialNode& material) {
    const auto nodeName = GetLocalNodeName(material.path);
    const AtString nodeType(material.identifier.GetText());
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
        ret = AiNode(_delegate->GetUniverse(), material.identifier.GetText());
        if (ret == nullptr) { return nullptr; }
        AiNodeSetStr(ret, nameName, nodeName);
        _nodes.emplace(nodeName, ret);
    }

    const auto* nentry = AiNodeGetNodeEntry(ret);
    for (const auto& param : material.parameters) {
        const auto& paramName = param.first;
        const auto* pentry =
            AiNodeEntryLookUpParameter(nentry, AtString(paramName.GetText()));
        if (pentry == nullptr) { continue; }
        SetParameter(ret, pentry, param.second);
    }
    return ret;
}

AtString HdAiMaterial::GetLocalNodeName(const SdfPath& path) const {
    const auto* pp = path.GetText();
    if (pp == nullptr || pp[0] == '\0') { return AtString(path.GetText()); }
    const auto p = GetId().AppendPath(SdfPath(TfToken(pp + 1)));
    return AtString(p.GetText());
}

PXR_NAMESPACE_CLOSE_SCOPE
