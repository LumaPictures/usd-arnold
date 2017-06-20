#include <maya/MFnPlugin.h>
#include <maya/MItDependencyNodes.h>

#include <usdMaya/shadingModeExporter.h>
#include <usdMaya/shadingModeRegistry.h>

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
                  PxrUsdMayaUtil::MDagPathMap<SdfPath>::Type& dagPathToUsdMap) {
        ArnoldShaderExport ai(stage, UsdTimeCode::Default(), "", dagPathToUsdMap);
        if (bindableRoots.empty()) {
            for (MItDependencyNodes iter(MFn::kShadingEngine); !iter.isDone(); iter.next()) {
                MObject obj = iter.thisNode();
                const auto exportedShader = ai.export_shader(obj);
            }
        }
        ai.setup_shaders();
    }
};

TF_REGISTRY_FUNCTION_WITH_TAG(PxrUsdMayaShadingModeExportContext, pxrRis)
{
    PxrUsdMayaShadingModeRegistry::GetInstance().RegisterExporter("arnold", []() -> PxrUsdMayaShadingModeExporterPtr {
        return PxrUsdMayaShadingModeExporterPtr(
            static_cast<PxrUsdMayaShadingModeExporter*>(new ArnoldShadingModeExporter()));
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
