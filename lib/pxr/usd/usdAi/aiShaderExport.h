#ifndef USDAI_SHADER_EXPORT_H
#define USDAI_SHADER_EXPORT_H

#include "pxr/usd/usdAi/aiShader.h"

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class AiShaderExport {
public:
    AiShaderExport(const UsdStagePtr& _stage,
                   const UsdTimeCode& _time_code = UsdTimeCode::Default(),
                   const std::string& parent_scope = std::string());
    ~AiShaderExport();
    void bind_material(const SdfPath& shader_path, const SdfPath& shape_path);
    SdfPath export_material(const char* material_name,
                            AtNode* surf_shader, AtNode* disp_shader=nullptr);
    SdfPath export_arnold_node(const AtNode* arnold_node,
                               SdfPath parent_path);
    static void clean_arnold_name(std::string& name);
    bool get_output_parameter(const AtNode* arnold_node,
                              uint8_t arnold_param_type, int32_t comp_index, UsdShadeOutput& out);
    void export_connection(const AtNode* arnold_node, UsdAiShader& shader,
                           const char* arnold_param_name, uint8_t arnold_param_type);
    void export_parameter(const AtNode* arnold_node, UsdAiShader& shader,
                          const char* arnold_param_name,
                          uint8_t arnold_param_type, bool user);
    void collapse_shaders();

protected:
    const UsdStagePtr m_stage;
    SdfPath m_shaders_scope;
    UsdTimeCode m_time_code;

private:
    std::map<const AtNode*, SdfPath> m_shader_to_usd_path;

};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
