// Copyright (c) 2019 Luma Pictures . All rights reserved.
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
#ifndef USDMAYA_ARNOLD_SHADER_EXPORT_H
#define USDMAYA_ARNOLD_SHADER_EXPORT_H

#include <pxr/usd/usdAi/aiShaderExport.h>

#include <usdMaya/util.h>

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
