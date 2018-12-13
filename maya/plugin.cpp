// Copyright (c) 2018 Luma Pictures . All rights reserved.
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
#include <maya/MFnPlugin.h>
#include <maya/MItDependencyNodes.h>

#include <usdMaya/shadingModeExporter.h>
#include <usdMaya/shadingModeRegistry.h>
#include <usdMaya/userAttributeWriterRegistry.h>

#include "ArnoldShaderExport.h"

PXR_NAMESPACE_USING_DIRECTIVE

class ArnoldShadingModeExporter : public UsdMayaShadingModeExporter {
public:
    ArnoldShadingModeExporter() {}

    ~ArnoldShadingModeExporter() { delete ai; }

    void PreExport(UsdMayaShadingModeExportContext* context) override {
        ai = new ArnoldShaderExport(
            context->GetUsdStage(), UsdTimeCode::Default(),
            context->GetExportArgs().stripNamespaces,
            context->GetExportArgs().parentScope,
            context->GetDagPathToUsdMap());
    }

    void Export(
        const UsdMayaShadingModeExportContext& context,
        UsdShadeMaterial* const mat,
        SdfPathSet* const boundPrimPaths) override {
        if (ai != nullptr) {
            ai->export_shading_engine(context.GetShadingEngine());
        }
    }

    void PostExport(const UsdMayaShadingModeExportContext& context) override {
        if (ai != nullptr) {
            ai->setup_shaders();
            delete ai;
            ai = nullptr;
        }
    }

private:
    ArnoldShaderExport* ai;
};

TF_REGISTRY_FUNCTION_WITH_TAG(UsdMayaShadingModeExportContext, arnold) {
    UsdMayaShadingModeRegistry::GetInstance().RegisterExporter(
        "arnold", []() -> UsdMayaShadingModeExporterPtr {
            return UsdMayaShadingModeExporterPtr(
                static_cast<UsdMayaShadingModeExporter*>(
                    new ArnoldShadingModeExporter()));
        });
}

TF_REGISTRY_FUNCTION_WITH_TAG(UsdMayaUserAttributeWriterRegistry, usdAi) {
    UsdMayaUserAttributeWriterRegistry::RegisterWriter(
        "usdAi",
        [](const MPlug& attrPlug, const UsdPrim& usdPrim,
           const std::string& attrName, const std::string& nameSpace,
           const bool translateMayaDoubleToUsdSinglePrecision) -> UsdAttribute {
            return UsdAttribute();
        });
}

MStatus initializePlugin(MObject obj) {
    // TODO: Make sure MtoA can be loaded.
    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) { return MS::kSuccess; }
