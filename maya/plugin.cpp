#include <maya/MFnPlugin.h>
#include <maya/MItDependencyNodes.h>

#include <usdMaya/shadingModeExporter.h>
#include <usdMaya/shadingModeRegistry.h>
#include <usdMaya/userAttributeWriterRegistry.h>

#include "ArnoldShaderExport.h"

PXR_NAMESPACE_USING_DIRECTIVE

class ArnoldShadingModeExporter : public UsdMayaShadingModeExporter {
public:
    ArnoldShadingModeExporter() {

    }

    ~ArnoldShadingModeExporter() {
        delete ai;
    }

    void PreExport(UsdMayaShadingModeExportContext* context) override
    {
        ai = new ArnoldShaderExport(
                context->GetUsdStage(),
                UsdTimeCode::Default(),
                context->GetStripNamespaces(),
                context->GetParentScope(),
                context->GetDagPathToUsdMap());
    }

    void Export(const UsdMayaShadingModeExportContext& context,
                UsdShadeMaterial * const mat,
                SdfPathSet * const boundPrimPaths) override
    {
        if (ai != nullptr)
        {
            ai->export_shading_engine(context.GetShadingEngine());
        }
    }

    void PostExport(const UsdMayaShadingModeExportContext& context) override
    {
        if (ai != nullptr)
        {
            ai->setup_shaders();
            delete ai;
            ai = nullptr;
        }
    }

private:
    ArnoldShaderExport* ai;
};

TF_REGISTRY_FUNCTION_WITH_TAG(UsdMayaShadingModeExportContext, arnold) {
    UsdMayaShadingModeRegistry::GetInstance().RegisterExporter("arnold", []() -> UsdMayaShadingModeExporterPtr {
        return UsdMayaShadingModeExporterPtr(
            static_cast<UsdMayaShadingModeExporter*>(new ArnoldShadingModeExporter()));
    });
}

TF_REGISTRY_FUNCTION_WITH_TAG(UsdMayaUserAttributeWriterRegistry, usdAi) {
    UsdMayaUserAttributeWriterRegistry::RegisterWriter("usdAi", [](
        const MPlug& attrPlug,
        const UsdPrim& usdPrim,
        const std::string& attrName,
        const std::string& nameSpace,
        const bool translateMayaDoubleToUsdSinglePrecision) -> UsdAttribute {
        return UsdAttribute();
    });
}

MStatus initializePlugin(MObject obj)
{
    // TODO: Make sure MtoA can be loaded.
    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
    return MS::kSuccess;
}
