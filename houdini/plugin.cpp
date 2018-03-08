// This is just to suppress a warning message from Houdini.
// The plugin works without this.

#include <SYS/SYS_Version.h>
#include <UT/UT_DSOVersion.h>
#include <OP/OP_OperatorTable.h>

extern "C" {
    // An empty function so houdini won't complain about missing symbols.
    void newDriverOperator(OP_OperatorTable* operators) 
    {
    }
}

#include <gusd/shadingModeRegistry.h>
#include <gusd/primWrapper.h>

#include "arnoldShadingExporter.h"

PXR_NAMESPACE_USING_DIRECTIVE

TF_REGISTRY_FUNCTION_WITH_TAG(GusdShadingModeRegistry, arnold) {
    GusdShadingModeRegistry::getInstance().registerExporter(
        "arnold", "Arnold", &exportArnoldShaders);
}
