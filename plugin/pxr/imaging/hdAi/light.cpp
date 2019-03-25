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
#include "pxr/imaging/hdAi/light.h"

#include "pxr/imaging/hdAi/material.h"
#include "pxr/imaging/hdAi/utils.h"

#include <pxr/usd/usdLux/tokens.h>

#include <pxr/usd/sdf/assetPath.h>

#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

namespace {

const AtString pointLightType("point_light");
const AtString distantLightType("distant_light");
const AtString diskLightType("disk_light");
const AtString rectLightType("quad_light");
const AtString cylinderLightType("cylinder_light");
const AtString domeLightType("skydome_light");

const AtString formatStr("format");
const AtString angularStr("angular");
const AtString latlongStr("latlong");
const AtString mirroredBallStr("mirrored_ball");

const AtString shaderStr("shader");
const AtString imageStr("image");
const AtString filenameStr("filename");
const AtString colorStr("color");

struct ParamDesc {
    ParamDesc(const char* aname, const TfToken& hname)
        : arnoldName(aname), hdName(hname) {}
    AtString arnoldName;
    TfToken hdName;
};

std::vector<ParamDesc> genericParams = {
    {"intensity", HdLightTokens->intensity},
    {"exposure", HdLightTokens->exposure},
    {"color", HdLightTokens->color},
    {"diffuse", HdLightTokens->diffuse},
    {"specular", HdLightTokens->specular},
    {"normalize", HdLightTokens->normalize},
    {"cast_shadows", HdLightTokens->shadowEnable},
    {"shadow_color", HdLightTokens->shadowColor},
};

std::vector<ParamDesc> pointParams = {{"radius", HdLightTokens->radius}};

std::vector<ParamDesc> distantParams = {{"angle", HdLightTokens->angle}};

std::vector<ParamDesc> diskParams = {{"radius", HdLightTokens->radius}};

std::vector<ParamDesc> cylinderParams = {{"radius", HdLightTokens->radius}};

void iterateParams(
    AtNode* light, const AtNodeEntry* nentry, const SdfPath& id,
    HdSceneDelegate* delegate, const std::vector<ParamDesc>& params) {
    for (const auto& param : params) {
        const auto* pentry =
            AiNodeEntryLookUpParameter(nentry, param.arnoldName);
        if (pentry == nullptr) { continue; }
        HdAiSetParameter(
            light, pentry, delegate->GetLightParamValue(id, param.hdName));
    }
}

auto pointLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                         const SdfPath& id, HdSceneDelegate* delegate) {
    iterateParams(light, nentry, id, delegate, pointParams);
};

auto distantLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                           const SdfPath& id, HdSceneDelegate* delegate) {
    iterateParams(light, nentry, id, delegate, distantParams);
};

auto diskLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                        const SdfPath& id, HdSceneDelegate* delegate) {
    iterateParams(light, nentry, id, delegate, diskParams);
};

auto rectLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                        const SdfPath& id, HdSceneDelegate* delegate) {
    float width = 1.0f;
    float height = 1.0f;
    const auto& widthValue =
        delegate->GetLightParamValue(id, HdLightTokens->width);
    if (widthValue.IsHolding<float>()) {
        width = widthValue.UncheckedGet<float>();
    }
    const auto& heightValue =
        delegate->GetLightParamValue(id, HdLightTokens->height);
    if (heightValue.IsHolding<float>()) {
        height = heightValue.UncheckedGet<float>();
    }

    width /= 2.0f;
    height /= 2.0f;

    AiNodeSetArray(
        light, "vertices",
        AiArray(
            4, 1, AI_TYPE_VECTOR, AtVector(-width, height, 0.0f),
            AtVector(width, height, 0.0f), AtVector(width, -height, 0.0f),
            AtVector(-width, -height, 0.0f)));
};

auto cylinderLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                            const SdfPath& id, HdSceneDelegate* delegate) {
    iterateParams(light, nentry, id, delegate, cylinderParams);
    float length = 1.0f;
    const auto& lengthValue =
        delegate->GetLightParamValue(id, UsdLuxTokens->length);
    if (lengthValue.IsHolding<float>()) {
        length = lengthValue.UncheckedGet<float>();
    }
    length /= 2.0f;
    AiNodeSetVec(light, "bottom", 0.0f, -length, 0.0f);
    AiNodeSetVec(light, "top", 0.0f, length, 0.0f);
};

auto domeLightSync = [](AtNode* light, const AtNodeEntry* nentry,
                        const SdfPath& id, HdSceneDelegate* delegate) {
    const auto& formatValue =
        delegate->GetLightParamValue(id, UsdLuxTokens->textureFormat);
    if (formatValue.IsHolding<TfToken>()) {
        const auto& textureFormat = formatValue.UncheckedGet<TfToken>();
        if (textureFormat == UsdLuxTokens->latlong) {
            AiNodeSetStr(light, formatStr, latlongStr);
        } else if (textureFormat == UsdLuxTokens->mirroredBall) {
            AiNodeSetStr(light, formatStr, mirroredBallStr);
        } else {
            AiNodeSetStr(light, formatStr, angularStr); // default value    
        }
    }
};

} // namespace

HdAiLight* HdAiLight::CreatePointLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, pointLightType, pointLightSync);
}

HdAiLight* HdAiLight::CreateDistantLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, distantLightType, distantLightSync);
}

HdAiLight* HdAiLight::CreateDiskLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, diskLightType, diskLightSync);
}

HdAiLight* HdAiLight::CreateRectLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, rectLightType, rectLightSync, true);
}

HdAiLight* HdAiLight::CreateCylinderLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, cylinderLightType, cylinderLightSync);
}

HdAiLight* HdAiLight::CreateDomeLight(
    HdAiRenderDelegate* delegate, const SdfPath& id) {
    return new HdAiLight(delegate, id, domeLightType, domeLightSync, true);
}

void HdAiLight::Sync(
    HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
    HdDirtyBits* dirtyBits) {
    auto* param = reinterpret_cast<HdAiRenderParam*>(renderParam);
    TF_UNUSED(sceneDelegate);
    TF_UNUSED(dirtyBits);
    if (*dirtyBits & HdLight::DirtyParams) {
        param->End();
        const auto id = GetId();
        const auto* nentry = AiNodeGetNodeEntry(_light);
        iterateParams(_light, nentry, id, sceneDelegate, genericParams);
        _syncParams(_light, nentry, id, sceneDelegate);
        if (_supportsTexture) {
            SetupTexture(sceneDelegate->GetLightParamValue(
                id, HdLightTokens->textureFile));
        }
    }

    if (*dirtyBits & HdLight::DirtyTransform) {
        param->End();
        HdAiSetTransform(_light, sceneDelegate, GetId());
    }
    *dirtyBits = HdLight::Clean;
}

void HdAiLight::SetupTexture(const VtValue& value) {
    const auto* nentry = AiNodeGetNodeEntry(_light);
    const auto hasShader =
        AiNodeEntryLookUpParameter(nentry, shaderStr) != nullptr;
    if (hasShader) {
        AiNodeSetPtr(_light, shaderStr, nullptr);
    } else {
        AiNodeUnlink(_light, colorStr);
    }
    if (_texture != nullptr) {
        AiNodeDestroy(_texture);
        _texture = nullptr;
    }
    if (!value.IsHolding<SdfAssetPath>()) { return; }
    const auto& assetPath = value.UncheckedGet<SdfAssetPath>();
    auto path = assetPath.GetResolvedPath();
    if (path.empty()) { path = assetPath.GetAssetPath(); }

    if (path.empty()) { return; }
    _texture = AiNode(_delegate->GetUniverse(), imageStr);
    AiNodeSetStr(_texture, filenameStr, path.c_str());
    if (hasShader) {
        AiNodeSetPtr(_light, shaderStr, _texture);
    } else { // Connect to color if filename doesn't exists.
        AiNodeLink(_texture, colorStr, _light);
    }
}

HdDirtyBits HdAiLight::GetInitialDirtyBitsMask() const {
    return HdLight::DirtyParams | HdLight::DirtyTransform;
}

HdAiLight::HdAiLight(
    HdAiRenderDelegate* delegate, const SdfPath& id, const AtString& arnoldType,
    const HdAiLight::SyncParams& sync, bool supportsTexture)
    : HdLight(id),
      _syncParams(sync),
      _delegate(delegate),
      _supportsTexture(supportsTexture) {
    _light = AiNode(_delegate->GetUniverse(), arnoldType);
    if (id.IsEmpty()) {
        AiNodeSetFlt(_light, "intensity", 0.0f);
    } else {
        AiNodeSetStr(_light, "name", id.GetText());
    }
}

HdAiLight::~HdAiLight() {
    AiNodeDestroy(_light);
    if (_texture != nullptr) { AiNodeDestroy(_texture); }
}

PXR_NAMESPACE_CLOSE_SCOPE
