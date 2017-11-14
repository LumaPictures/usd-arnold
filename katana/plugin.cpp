//.#include "pxrUsdInShipped/declareCoreOps.h"


#include <usdKatana/attrMap.h>
#include <usdKatana/usdInPluginRegistry.h>

#include <pxr/usd/usdAi/aiProcedural.h>
#include <pxr/usd/usdAi/aiVolume.h>
#include <pxr/usd/usdAi/aiAOV.h>

#include "readProcedural.h"
#include "readPrim.h"

PXR_NAMESPACE_USING_DIRECTIVE

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(AiProceduralOp)
DEFINE_GEOLIBOP_PLUGIN(AiProceduralOp)
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(AiProceduralOp, privateData, opArgs, interface)
{
    PxrUsdKatanaAttrMap attrs;

    readAiProcedural(
        UsdAiProcedural(privateData.GetUsdPrim()), privateData, attrs);

    attrs.toInterface(interface);
}

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(UsdArnold_LocationDecorator)
DEFINE_GEOLIBOP_PLUGIN(UsdArnold_LocationDecorator);
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(UsdArnold_LocationDecorator,
    privateData, opArgs, interface)
{
    readPrimLocation(interface, opArgs, privateData);
}

void registerPlugins()
{
    USD_OP_REGISTER_PLUGIN(AiProceduralOp, "AiProceduralOp", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiProcedural>("AiProceduralOp");
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiVolume>("AiProceduralOp");

    USD_OP_REGISTER_PLUGIN(UsdArnold_LocationDecorator, "UsdArnold_LocationDecorator", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterLocationDecoratorOp("UsdArnold_LocationDecorator");
}
