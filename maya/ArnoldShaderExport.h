#ifndef USDMAYA_ARNOLD_SHADER_EXPORT_H
#define USDMAYA_ARNOLD_SHADER_EXPORT_H

#include <pxr/usd/usdAi/aiShaderExport.h>

#include <usdMaya/util.h>

#include <maya/MObject.h>
#include <maya/MDagPath.h>

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class ArnoldShaderExport : public AiShaderExport {
public:
    ArnoldShaderExport(const UsdStageRefPtr& _stage, const UsdTimeCode& _time_code, const SdfPath& parent_scope,
                       const PxrUsdMayaUtil::MDagPathMap<SdfPath>::Type& dag_to_usd);
    virtual ~ArnoldShaderExport();

private:
    enum TransformAssignment{
        TRANSFORM_ASSIGNMENT_DISABLE,
        TRANSFORM_ASSIGNMENT_COMMON,
        TRANSFORM_ASSIGNMENT_FULL
    };
    TransformAssignment m_transform_assignment;
    const PxrUsdMayaUtil::MDagPathMap<SdfPath>::Type& m_dag_to_usd;

    void setup_shader(const MDagPath& dg, const SdfPath& path);
public:
    SdfPath export_shading_engine(MObject obj);
    void setup_shaders();
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
