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
#ifndef USDMAYA_ARNOLD_SHADER_EXPORT_H
#define USDMAYA_ARNOLD_SHADER_EXPORT_H

#include <pxr/usd/usdAi/aiShaderExport.h>

#include <mayaUsd/utils/util.h>

#include <maya/MDagPath.h>
#include <maya/MObject.h>

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class ArnoldShaderExport : public AiShaderExport {
public:
    ArnoldShaderExport(
        const UsdStageRefPtr& _stage, const UsdTimeCode& _time_code,
        bool strip_namespaces, const SdfPath& parent_scope,
        const UsdMayaUtil::MDagPathMap<SdfPath>& dag_to_usd);
    virtual ~ArnoldShaderExport();

private:
    enum TransformAssignment {
        TRANSFORM_ASSIGNMENT_DISABLE,
        TRANSFORM_ASSIGNMENT_COMMON,
        TRANSFORM_ASSIGNMENT_FULL
    };
    TransformAssignment m_transform_assignment;
    const UsdMayaUtil::MDagPathMap<SdfPath>& m_dag_to_usd;

    void setup_shader(const MDagPath& dg, const SdfPath& path);

public:
    SdfPath export_shading_engine(MObject obj);
    void setup_shaders();

    bool m_strip_namespaces;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
