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

    }

    void DoExport(const UsdStageRefPtr& stage,
                  const PxrUsdMayaUtil::ShapeSet& bindableRoots,
                  bool mergeTransformAndShape,
                  const SdfPath& overrideRootPath,
                  const std::string& parentScope,
                  const PxrUsdMayaUtil::MDagPathMap<SdfPath>::Type& dagPathToUsdMap) override {
        ArnoldShaderExport ai(stage, UsdTimeCode::Default(), parentScope, dagPathToUsdMap);
        if (bindableRoots.empty()) {
            for (MItDependencyNodes iter(MFn::kShadingEngine); !iter.isDone(); iter.next()) {
                MObject obj = iter.thisNode();
                const auto exportedShader = ai.export_shading_engine(obj);
            }
        }
        ai.setup_shaders();
    }
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
