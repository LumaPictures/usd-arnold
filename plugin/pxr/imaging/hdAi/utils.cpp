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
#include "pxr/imaging/hdAi/utils.h"

#include <pxr/base/gf/vec2f.h>

#include <pxr/usd/sdf/assetPath.h>

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PRIVATE_TOKENS(
    _tokens, (BOOL)(BYTE)(INT)(UINT)(FLOAT)(VECTOR2)(VECTOR)(RGB)(RGBA)(STRING)(
                 constant)(uniform)(varying)(indexed)(
                 (constantArray, "constant ARRAY")));

namespace {

inline bool _Declare(
    AtNode* node, const TfToken& name, const TfToken& scope,
    const TfToken& type) {
    return AiNodeDeclare(
        node, name.GetText(),
        TfStringPrintf("%s %s", scope.GetText(), type.GetText()).c_str());
}

template <typename T>
inline uint32_t _DeclareAndConvertArray(
    AtNode* node, const TfToken& name, const TfToken& scope,
    const TfToken& type, uint8_t arnoldType, const VtValue& value) {
    if (!_Declare(node, name, scope, type)) { return 0; }
    const auto& v = value.UncheckedGet<T>();
    auto a = AiArrayConvert(v.size(), 1, arnoldType, v.data());
    AiNodeSetArray(node, name.GetText(), a);
    return AiArrayGetNumElements(a);
}

// This is useful for uniform, vertex and face-varying. We need to know the size
// to generate the indices for faceVarying data.
inline uint32_t _DeclareAndAssignFromArray(
    AtNode* node, const TfToken& name, const TfToken& scope,
    const VtValue& value, bool isColor = false) {
    if (value.IsHolding<VtBoolArray>()) {
        return _DeclareAndConvertArray<VtBoolArray>(
            node, name, scope, _tokens->BOOL, AI_TYPE_BOOLEAN, value);
    } else if (value.IsHolding<VtUCharArray>()) {
        return _DeclareAndConvertArray<VtUCharArray>(
            node, name, scope, _tokens->BYTE, AI_TYPE_BYTE, value);
    } else if (value.IsHolding<VtUIntArray>()) {
        return _DeclareAndConvertArray<VtUIntArray>(
            node, name, scope, _tokens->UINT, AI_TYPE_UINT, value);
    } else if (value.IsHolding<VtIntArray>()) {
        return _DeclareAndConvertArray<VtIntArray>(
            node, name, scope, _tokens->INT, AI_TYPE_INT, value);
    } else if (value.IsHolding<VtFloatArray>()) {
        return _DeclareAndConvertArray<VtFloatArray>(
            node, name, scope, _tokens->FLOAT, AI_TYPE_FLOAT, value);
    } else if (value.IsHolding<VtDoubleArray>()) {
        // TODO
    } else if (value.IsHolding<VtVec2fArray>()) {
        return _DeclareAndConvertArray<VtVec2fArray>(
            node, name, scope, _tokens->VECTOR2, AI_TYPE_VECTOR2, value);
    } else if (value.IsHolding<VtVec3fArray>()) {
        if (isColor) {
            return _DeclareAndConvertArray<VtVec3fArray>(
                node, name, scope, _tokens->RGB, AI_TYPE_RGB, value);
        } else {
            return _DeclareAndConvertArray<VtVec3fArray>(
                node, name, scope, _tokens->VECTOR, AI_TYPE_VECTOR, value);
        }
    } else if (value.IsHolding<VtVec4fArray>()) {
        return _DeclareAndConvertArray<VtVec4fArray>(
            node, name, scope, _tokens->RGBA, AI_TYPE_RGBA, value);
    }
    return 0;
}

inline
void _DeclareAndAssignConstant(
    AtNode* node, const TfToken& name, const VtValue& value,
    bool isColor = false) {
    auto declareConstant = [&node, &name] (const TfToken& type) -> bool {
        return _Declare(node, name, _tokens->constant, type);
    };
    if (value.IsHolding<bool>()) {
        if (!declareConstant(_tokens->BOOL)) { return; }
        AiNodeSetBool(node, name.GetText(), value.UncheckedGet<bool>());
    } else if (value.IsHolding<uint8_t>()) {
        if (!declareConstant(_tokens->BYTE)) { return; }
        AiNodeSetByte(node, name.GetText(), value.UncheckedGet<uint8_t>());
    } else if (value.IsHolding<unsigned int>()) {
        if (!declareConstant(_tokens->UINT)) { return; }
        AiNodeSetUInt(node, name.GetText(), value.UncheckedGet<unsigned int>());
    } else if (value.IsHolding<int>()) {
        if (!declareConstant(_tokens->INT)) { return; }
        AiNodeSetInt(node, name.GetText(), value.UncheckedGet<int>());
    } else if (value.IsHolding<float>()) {
        if (!declareConstant(_tokens->FLOAT)) { return; }
        AiNodeSetFlt(node, name.GetText(), value.UncheckedGet<float>());
    } else if (value.IsHolding<double>()) {
        if (!declareConstant(_tokens->FLOAT)) { return; }
        AiNodeSetFlt(node, name.GetText(),
            static_cast<float>(value.UncheckedGet<double>()));
    } else if (value.IsHolding<GfVec2f>()) {
        if (!declareConstant(_tokens->VECTOR2)) { return; }
        const auto& v = value.UncheckedGet<GfVec2f>();
        AiNodeSetVec2(node, name.GetText(), v[0], v[1]);
    } else if (value.IsHolding<GfVec3f>()) {
        if (isColor) {
            if (!declareConstant(_tokens->RGB)) { return; }
            const auto& v = value.UncheckedGet<GfVec3f>();
            AiNodeSetRGB(node, name.GetText(), v[0], v[1], v[2]);
        } else {
            if (!declareConstant(_tokens->VECTOR)) { return; }
            const auto& v = value.UncheckedGet<GfVec3f>();
            AiNodeSetVec(node, name.GetText(), v[0], v[1], v[2]);
        }
    } else if (value.IsHolding<GfVec4f>()) {
        if (!declareConstant(_tokens->RGBA)) { return; }
        const auto& v = value.UncheckedGet<GfVec4f>();
        AiNodeSetRGBA(node, name.GetText(), v[0], v[1], v[2], v[3]);
    } else {
        _DeclareAndAssignFromArray(
            node, name, _tokens->constantArray, value, isColor);
    }
}

} // namespace

AtMatrix HdAiConvertMatrix(const GfMatrix4d& in) {
    AtMatrix out = AI_M4_IDENTITY;
    out.data[0][0] = static_cast<float>(in[0][0]);
    out.data[0][1] = static_cast<float>(in[0][1]);
    out.data[0][2] = static_cast<float>(in[0][2]);
    out.data[0][3] = static_cast<float>(in[0][3]);
    out.data[1][0] = static_cast<float>(in[1][0]);
    out.data[1][1] = static_cast<float>(in[1][1]);
    out.data[1][2] = static_cast<float>(in[1][2]);
    out.data[1][3] = static_cast<float>(in[1][3]);
    out.data[2][0] = static_cast<float>(in[2][0]);
    out.data[2][1] = static_cast<float>(in[2][1]);
    out.data[2][2] = static_cast<float>(in[2][2]);
    out.data[2][3] = static_cast<float>(in[2][3]);
    out.data[3][0] = static_cast<float>(in[3][0]);
    out.data[3][1] = static_cast<float>(in[3][1]);
    out.data[3][2] = static_cast<float>(in[3][2]);
    out.data[3][3] = static_cast<float>(in[3][3]);
    return out;
}

AtMatrix HdAiConvertMatrix(const GfMatrix4f& in) {
    AtMatrix out = AI_M4_IDENTITY;
    out.data[0][0] = in[0][0];
    out.data[0][1] = in[0][1];
    out.data[0][2] = in[0][2];
    out.data[0][3] = in[0][3];
    out.data[1][0] = in[1][0];
    out.data[1][1] = in[1][1];
    out.data[1][2] = in[1][2];
    out.data[1][3] = in[1][3];
    out.data[2][0] = in[2][0];
    out.data[2][1] = in[2][1];
    out.data[2][2] = in[2][2];
    out.data[2][3] = in[2][3];
    out.data[3][0] = in[3][0];
    out.data[3][1] = in[3][1];
    out.data[3][2] = in[3][2];
    out.data[3][3] = in[3][3];
    return out;
}

GfMatrix4f HdAiConvertMatrix(const AtMatrix& in) {
    GfMatrix4f out(1.0f);
    out[0][0] = in.data[0][0];
    out[0][1] = in.data[0][1];
    out[0][2] = in.data[0][2];
    out[0][3] = in.data[0][3];
    out[1][0] = in.data[1][0];
    out[1][1] = in.data[1][1];
    out[1][2] = in.data[1][2];
    out[1][3] = in.data[1][3];
    out[2][0] = in.data[2][0];
    out[2][1] = in.data[2][1];
    out[2][2] = in.data[2][2];
    out[2][3] = in.data[2][3];
    out[3][0] = in.data[3][0];
    out[3][1] = in.data[3][1];
    out[3][2] = in.data[3][2];
    out[3][3] = in.data[3][3];
    return out;
}

void HdAiSetTransform(
    AtNode* node, HdSceneDelegate* delegate, const SdfPath& id) {
    constexpr size_t maxSamples = 3;
    HdTimeSampleArray<GfMatrix4d, maxSamples> xf;
    delegate->SampleTransform(id, &xf);
    AtArray* matrices = AiArrayAllocate(1, xf.count, AI_TYPE_MATRIX);
    for (auto i = decltype(xf.count){0}; i < xf.count; ++i) {
        AiArraySetMtx(matrices, i, HdAiConvertMatrix(xf.values[i]));
    }
    AiNodeSetArray(node, "matrix", matrices);
}

void HdAiSetTransform(
    std::vector<AtNode*>& nodes, HdSceneDelegate* delegate, const SdfPath& id) {
    constexpr size_t maxSamples = 3;
    HdTimeSampleArray<GfMatrix4d, maxSamples> xf;
    delegate->SampleTransform(id, &xf);
    AtArray* matrices = AiArrayAllocate(1, xf.count, AI_TYPE_MATRIX);
    for (auto i = decltype(xf.count){0}; i < xf.count; ++i) {
        AiArraySetMtx(matrices, i, HdAiConvertMatrix(xf.values[i]));
    }
    const auto nodeCount = nodes.size();
    if (nodeCount > 0) {
        // IIRC you can't set the same array on two different nodes,
        // because it causes a double-free.
        // TODO: we need to check if it's still the case with Arnold 5.
        for (auto i = decltype(nodeCount){1}; i < nodeCount; ++i) {
            AiNodeSetArray(nodes[i], "matrix", AiArrayCopy(matrices));
        }
        AiNodeSetArray(nodes[0], "matrix", matrices);
    }
}

void HdAiSetParameter(
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
            } else if (value.IsHolding<SdfAssetPath>()) {
                const auto& assetPath = value.UncheckedGet<SdfAssetPath>();
                AiNodeSetStr(
                    node, paramName,
                    assetPath.GetResolvedPath().empty()
                        ? assetPath.GetAssetPath().c_str()
                        : assetPath.GetResolvedPath().c_str());
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

void HdAiSetConstantPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc) {
    const auto isColor = primvarDesc.role == HdPrimvarRoleTokens->color;
    if (primvarDesc.name == HdTokens->color && isColor) {
        if (!_Declare(
                node, primvarDesc.name, _tokens->constant, _tokens->RGBA)) {
            return;
        }
        const auto value = delegate->Get(id, primvarDesc.name);
        if (value.IsHolding<GfVec4f>()) {
            const auto& v = value.UncheckedGet<GfVec4f>();
            AiNodeSetRGBA(
                node, primvarDesc.name.GetText(), v[0], v[1], v[2], v[3]);
        } else if (value.IsHolding<VtVec4fArray>()) {
            const auto& arr = value.UncheckedGet<VtVec4fArray>();
            if (arr.empty()) { return; }
            const auto& v = arr[0];
            AiNodeSetRGBA(
                node, primvarDesc.name.GetText(), v[0], v[1], v[2], v[3]);
        }
    }
    _DeclareAndAssignConstant(
        node, primvarDesc.name, delegate->Get(id, primvarDesc.name), isColor);
}

void HdAiSetUniformPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc) {
    _DeclareAndAssignFromArray(
        node, primvarDesc.name, _tokens->varying,
        delegate->Get(id, primvarDesc.name),
        primvarDesc.role == HdPrimvarRoleTokens->color);
}

void HdAiSetVertexPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc) {
    _DeclareAndAssignFromArray(
        node, primvarDesc.name, _tokens->varying,
        delegate->Get(id, primvarDesc.name),
        primvarDesc.role == HdPrimvarRoleTokens->color);
}

void HdAiSetFaceVaryingPrimvar(
    AtNode* node, const SdfPath& id, HdSceneDelegate* delegate,
    const HdPrimvarDescriptor& primvarDesc) {
    const auto numElements = _DeclareAndAssignFromArray(
        node, primvarDesc.name, _tokens->varying,
        delegate->Get(id, primvarDesc.name),
        primvarDesc.role == HdPrimvarRoleTokens->color);
    if (numElements != 0) {
        auto* a = AiArrayAllocate(numElements, 1, AI_TYPE_UINT);
        for (auto i = decltype(numElements){0}; i < numElements; ++i) {
            AiArraySetUInt(a, i, i);
        }
        AiNodeSetArray(
            node, TfStringPrintf("%sidsx", primvarDesc.name.GetText()).c_str(),
            a);
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
