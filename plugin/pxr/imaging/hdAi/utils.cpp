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
#include "pxr/imaging/hdAi/utils.h"

#include <pxr/base/gf/vec2f.h>

PXR_NAMESPACE_OPEN_SCOPE

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

PXR_NAMESPACE_CLOSE_SCOPE
