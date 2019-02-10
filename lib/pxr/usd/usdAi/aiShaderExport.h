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
#ifndef USDAI_SHADER_EXPORT_H
#define USDAI_SHADER_EXPORT_H

#include "pxr/usd/usdAi/aiShader.h"

struct AtNode;
struct AtParamEntry;
class AtParamValue;
class AtArray;

PXR_NAMESPACE_OPEN_SCOPE

class AiShaderExport {
public:
    AiShaderExport(
        const UsdStagePtr& _stage, const SdfPath& parent_scope = SdfPath(),
        const UsdTimeCode& _time_code = UsdTimeCode::Default());
    virtual ~AiShaderExport() = default;
    void bind_material(const SdfPath& shader_path, const SdfPath& shape_path);
    SdfPath export_material(
        const char* material_name, AtNode* surf_shader,
        AtNode* disp_shader = nullptr);
    SdfPath export_arnold_node(
        const AtNode* arnold_node, const SdfPath& parent_path,
        const std::set<std::string>* exportable_params = nullptr);
    static void clean_arnold_name(std::string& name);
    bool get_output(
        const AtNode* src_arnold_node, UsdAiShader& src_shader,
        UsdShadeOutput& out, bool is_node_type = false,
        int32_t comp_index = -1);
    bool export_connection(
        const AtNode* dest_arnold_node, UsdAiShader& dest_shader,
        const std::string& dest_param_name,
        const std::string& dest_param_arnold_name, uint8_t arnold_param_type);
    bool export_connection(
        const AtNode* dest_arnold_node, UsdAiShader& dest_shader,
        const char* dest_param_name, const AtNode* src_arnold_node,
        UsdAiShader& src_shader, int32_t src_comp_index = -1);
    // bool export_connection(const AtNode* dest_arnold_node, UsdAiShader&
    // dest_shader,
    //                        const char* dest_param_name, uint8_t
    //                        arnold_param_type, const AtNode* src_arnold_node,
    //                        UsdAiShader& src_shader, int32_t
    //                        src_comp_index=-1);
    void export_parameter(
        const AtNode* arnold_node, UsdAiShader& shader,
        const char* arnold_param_name, uint8_t arnold_param_type, bool user);
    void collapse_shaders();

public:
    // Utility functions and definitions.
    struct ParamConversion {
        const SdfValueTypeName& type;
        std::function<VtValue(const AtNode*, const char*)> f;

        // TODO: see if move works in this case.
        ParamConversion(
            const SdfValueTypeName& _type,
            std::function<VtValue(const AtNode*, const char*)> _f)
            : type(_type), f(std::move(_f)) {}
    };

    static const ParamConversion* get_param_conversion(uint8_t type);

    struct DefaultValueConversion {
        const SdfValueTypeName& type;
        std::function<VtValue(const AtParamValue&, const AtParamEntry*)> f;

        // The signature is pretty heavy here. The reason for that is simple
        // when dealing with metadatas, we don't have a valid AtParamEntry
        // structure. So we have to make all the behavior optional, otherwise we
        // would need to duplicate functions.
        DefaultValueConversion(
            const SdfValueTypeName& _type,
            std::function<VtValue(const AtParamValue&, const AtParamEntry*)> _f)
            : type(_type), f(std::move(_f)) {}
    };

    static const DefaultValueConversion* get_default_value_conversion(
        uint8_t type);

    struct ArrayConversion {
        const SdfValueTypeName& type;
        std::function<VtValue(const AtArray*)> f;

        ArrayConversion(
            const SdfValueTypeName& _type,
            std::function<VtValue(const AtArray*)> _f)
            : type(_type), f(std::move(_f)) {}
    };

    static const ArrayConversion* get_array_conversion(uint8_t type);

protected:
    const UsdStagePtr m_stage;
    SdfPath m_shaders_scope;
    UsdTimeCode m_time_code;

private:
    std::map<const AtNode*, SdfPath> m_shader_to_usd_path;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
