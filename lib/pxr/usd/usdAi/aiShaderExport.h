#ifndef USDAI_SHADER_EXPORT_H
#define USDAI_SHADER_EXPORT_H

#include "pxr/usd/usdAi/aiShader.h"

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class AiShaderExport {
public:
    AiShaderExport(const UsdStageRefPtr& _stage, const UsdTimeCode& _time_code, const std::string& parent_scope);
    ~AiShaderExport();

    static bool is_valid();
private:
    std::map<const AtNode*, SdfPath> m_shader_to_usd_path;
    
protected:
    const UsdStageRefPtr& m_stage;
    SdfPath m_shaders_scope;
    UsdTimeCode m_time_code;

    static void clean_arnold_name(std::string& name);
    virtual void begin();
    virtual void end();
    void export_parameter(const AtNode* arnold_node, UsdAiShader& shader, const char* arnold_param_name, uint8_t arnold_param_type, bool user);
    SdfPath write_arnold_node(const AtNode* arnold_node, SdfPath parent_path);
    void collapse_shaders();
public:
    void bind_material(const SdfPath& shader_path, const SdfPath& shape_path);
    SdfPath export_material(const char* material_name, AtNode* surf_shader, AtNode* disp_shader=nullptr);

};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
