#ifndef USDMAYA_ARNOLD_SHADER_EXPORT_H
#define USDMAYA_ARNOLD_SHADER_EXPORT_H

#include "pxr/usd/usdAi/aiShader.h"

#include "usdMaya/util.h"

#include <maya/MObject.h>
#include <maya/MDagPath.h>

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class ArnoldShaderExport {
public:
    ArnoldShaderExport(const UsdStageRefPtr& _stage, const UsdTimeCode& _time_code, const std::string& parent_scope,
                       const PxrUsdMayaUtil::MDagPathMap<SdfPath>::Type& dag_to_usd);
    ~ArnoldShaderExport();

    static bool is_valid();
private:
    enum TransformAssignment{
        TRANSFORM_ASSIGNMENT_DISABLE,
        TRANSFORM_ASSIGNMENT_COMMON,
        TRANSFORM_ASSIGNMENT_FULL
    };
    std::map<const AtNode*, SdfPath> m_shader_to_usd_path;
    const UsdStageRefPtr& m_stage;
    const PxrUsdMayaUtil::MDagPathMap<SdfPath>::Type& m_dag_to_usd;
    SdfPath m_shaders_scope;
    UsdTimeCode m_time_code;
    TransformAssignment m_transform_assignment;

    void export_parameter(const AtNode* arnold_node, UsdAiShader& shader, const char* arnold_param_name, uint8_t arnold_param_type, bool user);
    SdfPath write_arnold_node(const AtNode* arnold_node, SdfPath parent_path);
    void setup_shader(const MDagPath& dg, const SdfPath& path);
public:
    SdfPath export_shader(MObject obj);
    void setup_shaders();
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
