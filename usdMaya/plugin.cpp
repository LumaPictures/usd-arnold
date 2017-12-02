#include <maya/MFnPlugin.h>
#include <maya/MItDependencyNodes.h>

#include <usdMaya/shadingModeExporter.h>
#include <usdMaya/shadingModeRegistry.h>
#include <usdMaya/userAttributeWriterRegistry.h>

#include "ArnoldShaderExport.h"

PXR_NAMESPACE_USING_DIRECTIVE

class ArnoldShadingModeExporter : public PxrUsdMayaShadingModeExporter {
public:
    ArnoldShadingModeExporter() {

    }

    ~ArnoldShadingModeExporter() {
        if (ai != nullptr)
        {
            delete ai;
        }
    }

    void PreExport(const PxrUsdMayaShadingModeExportContext& context) override
    {
        ai = new ArnoldShaderExport(
                context.GetUsdStage(),
                UsdTimeCode::Default(),
                context.GetParentScope(),
                context.GetDagPathToUsdMap());
    }

    void Export(const PxrUsdMayaShadingModeExportContext& context,
                UsdShadeMaterial * const mat,
                SdfPathSet * const boundPrimPaths) override
    {
        if (ai != nullptr)
        {
            ai->export_shading_engine(context.GetShadingEngine());
        }
    }

    void PostExport(const PxrUsdMayaShadingModeExportContext& context) override
    {
        if (ai != nullptr)
        {
            ai->setup_shaders();
            delete ai;
        }
    }

private:
    ArnoldShaderExport* ai;
};

TF_REGISTRY_FUNCTION_WITH_TAG(PxrUsdMayaShadingModeExportContext, arnold) {
    PxrUsdMayaShadingModeRegistry::GetInstance().RegisterExporter("arnold", []() -> PxrUsdMayaShadingModeExporterPtr {
        return PxrUsdMayaShadingModeExporterPtr(
            static_cast<PxrUsdMayaShadingModeExporter*>(new ArnoldShadingModeExporter()));
    });
}

TF_REGISTRY_FUNCTION_WITH_TAG(PxrUsdMayaUserAttributeWriterRegistry, usdAi) {
    PxrUsdMayaUserAttributeWriterRegistry::RegisterWriter("usdAi", [](
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
