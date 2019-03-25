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
#include <ai.h>

// TODO: Use the SdrRegistry to get the default values.

AI_SHADER_NODE_EXPORT_METHODS(previewSurfaceMtd);

namespace {
enum params {
    p_diffuseColor,
    p_emissiveColor,
    p_useSpecularWorkflow,
    p_specularColor,
    p_metallic,
    p_roughness,
    p_clearcoat,
    p_clearcoatRoughness,
    p_opacity,
    p_ior,
    p_normal,
    p_displacement,
    p_occlusion
};
}

node_parameters {
    AiParameterRGB("diffuseColor", 0.18f, 0.18f, 0.18f);
    AiParameterRGB("emissiveColor", 0.0f, 0.0f, 0.0f);
    AiParameterInt("useSpecularWorkflow", 0);
    AiParameterRGB("specularColor", 0.0f, 0.0f, 0.0f);
    AiParameterFlt("metallic", 0.0f);
    AiParameterFlt("roughness", 0.5f);
    AiParameterFlt("clearcoat", 0.0f);
    AiParameterFlt("clearcoatRoughness", 0.01f);
    AiParameterFlt("opacity", 1.0f);
    AiParameterFlt("ior", 1.5f);
    AiParameterVec("normal", 0.0f, 0.0f, 1.0f);
    AiParameterFlt("displacement", 1.0f);
    AiParameterFlt("occlusion", 1.0f);

    AiMetaDataSetBool(nentry, "", "ndrai_dont_discover", true);
}

node_initialize {}

#include <iostream>

node_update {}

node_finish {}

shader_evaluate {
    auto& closures = sg->out.CLOSURE();
    const auto opacity = AiShaderEvalParamFlt(p_opacity);
    if (opacity < AI_OPACITY_EPSILON) {
        closures.add(AiClosureTransparent(sg, AI_RGB_WHITE));
        return;
    }

    if (sg->Rt & AI_RAY_SHADOW) {
        if (opacity < (1.0f - AI_OPACITY_EPSILON)) {
            closures.add(
                AiClosureTransparent(sg, AI_RGB_WHITE * (1.0f - opacity)));
        }
        return;
    }

    // TODO: implement the full shader.
    closures.add(AiOrenNayarBSDF(sg, AiShaderEvalParamRGB(p_diffuseColor), sg->Nf));
    const auto emissiveColor = AiShaderEvalParamRGB(p_emissiveColor);
    if (!AiColorIsSmall(emissiveColor)) {
        closures.add(AiClosureEmission(sg, emissiveColor));
    }

    if (opacity < (1.0f - AI_OPACITY_EPSILON)) {
        closures *= opacity;
        closures.add(AiClosureTransparent(sg, AI_RGB_WHITE * (1.0f - opacity)));
    }
}
