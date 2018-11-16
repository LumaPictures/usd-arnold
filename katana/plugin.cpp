//.#include "pxrUsdInShipped/declareCoreOps.h"


#include <usdKatana/attrMap.h>
#include <usdKatana/usdInPluginRegistry.h>

#include <pxr/usd/usdAi/aiAOV.h>
#include <pxr/usd/usdAi/aiProcedural.h>
#include <pxr/usd/usdAi/aiProceduralNode.h>
#include <pxr/usd/usdAi/aiVolume.h>

#include "readAOV.h"
#include "readProcedural.h"
#include "readPrim.h"
#include "readVolume.h"

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

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(AiVolumeOp)
DEFINE_GEOLIBOP_PLUGIN(AiVolumeOp)
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(AiVolumeOp, privateData, opArgs, interface)
{
    readVolume(interface, opArgs, privateData);
}

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(AiAOVOp)
DEFINE_GEOLIBOP_PLUGIN(AiAOVOp)
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(AiAOVOp, privateData, opArgs, interface)
{
    PxrUsdKatanaAttrMap attrs;

    readAiAOV(UsdAiAOV(privateData.GetUsdPrim()), privateData, attrs);

    attrs.toInterface(interface);

    // Tell the main `pxrUsdIn` op to skip the AOV child prims (the driver and
    // filter), since those are handled by `readAiAOV`.
    interface.setAttr("__UsdIn.skipAllChildren", FnAttribute::IntAttribute(1));
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
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiProceduralNode>("AiProceduralOp");
    USD_OP_REGISTER_PLUGIN(AiVolumeOp, "AiVolumeOp", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiVolume>("AiVolumeOp");
    USD_OP_REGISTER_PLUGIN(AiAOVOp, "AiAOVOp", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiAOV>("AiAOVOp");

    USD_OP_REGISTER_PLUGIN(UsdArnold_LocationDecorator, "UsdArnold_LocationDecorator", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterLocationDecoratorOp("UsdArnold_LocationDecorator");
}
