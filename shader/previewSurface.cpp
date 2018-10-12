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

node_update {}

node_finish {}

shader_evaluate {
    // TODO: implement the full shader.
    const auto diffuseColor = AiShaderEvalParamRGB(p_diffuseColor);
    if (!AiColorIsSmall(diffuseColor)) {
        sg->out.CLOSURE().add(AiOrenNayarBSDF(sg, diffuseColor, sg->Nf));
    }
    const auto emissiveColor = AiShaderEvalParamRGB(p_emissiveColor);
    if (!AiColorIsSmall(emissiveColor)) {
        sg->out.CLOSURE().add(AiClosureEmission(sg, emissiveColor));
    }
}
