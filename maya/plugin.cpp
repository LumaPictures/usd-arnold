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
