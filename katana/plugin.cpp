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
//.#include "pxrUsdInShipped/declareCoreOps.h"

#include <usdKatana/attrMap.h>
#include <usdKatana/usdInPluginRegistry.h>

#include <pxr/usd/usdVol/volume.h>

#include <pxr/usd/usdAi/aiAOV.h>
#include <pxr/usd/usdAi/aiProcedural.h>
#include <pxr/usd/usdAi/aiProceduralNode.h>
#include <pxr/usd/usdAi/aiVolume.h>

#include "readAOV.h"
#include "readAiVolume.h"
#include "readPrim.h"
#include "readProcedural.h"
#include "readUSDVolVolume.h"

PXR_NAMESPACE_USING_DIRECTIVE

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(AiProceduralOp)
DEFINE_GEOLIBOP_PLUGIN(AiProceduralOp)
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(
    AiProceduralOp, privateData, opArgs, interface) {
    PxrUsdKatanaAttrMap attrs;

    readAiProcedural(
        UsdAiProcedural(privateData.GetUsdPrim()), privateData, attrs);

    attrs.toInterface(interface);
}

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(AiVolumeOp)
DEFINE_GEOLIBOP_PLUGIN(AiVolumeOp)
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(AiVolumeOp, privateData, opArgs, interface) {
    readAiVolume(interface, opArgs, privateData);
}

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(AiAOVOp)
DEFINE_GEOLIBOP_PLUGIN(AiAOVOp)
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(AiAOVOp, privateData, opArgs, interface) {
    PxrUsdKatanaAttrMap attrs;

    readAiAOV(UsdAiAOV(privateData.GetUsdPrim()), privateData, attrs);

    attrs.toInterface(interface);

    // Tell the main `pxrUsdIn` op to skip the AOV child prims (the driver and
    // filter), since those are handled by `readAiAOV`.
    interface.setAttr("__UsdIn.skipAllChildren", FnAttribute::IntAttribute(1));
}

// Might need to rename this op if usdKatana grows support for usdVol
PXRUSDKATANA_USDIN_PLUGIN_DECLARE(USDVolVolumeOp)
DEFINE_GEOLIBOP_PLUGIN(USDVolVolumeOp)
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(
    USDVolVolumeOp, privateData, opArgs, interface) {
    readUSDVolVolume(interface, opArgs, privateData);
}

PXRUSDKATANA_USDIN_PLUGIN_DECLARE(UsdArnold_LocationDecorator)
DEFINE_GEOLIBOP_PLUGIN(UsdArnold_LocationDecorator);
PXRUSDKATANA_USDIN_PLUGIN_DEFINE(
    UsdArnold_LocationDecorator, privateData, opArgs, interface) {
    readPrimLocation(interface, opArgs, privateData);
}

void registerPlugins() {
    USD_OP_REGISTER_PLUGIN(AiProceduralOp, "AiProceduralOp", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiProcedural>(
        "AiProceduralOp");
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiProceduralNode>(
        "AiProceduralOp");
    USD_OP_REGISTER_PLUGIN(AiVolumeOp, "AiVolumeOp", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiVolume>("AiVolumeOp");
    USD_OP_REGISTER_PLUGIN(AiAOVOp, "AiAOVOp", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdAiAOV>("AiAOVOp");
    USD_OP_REGISTER_PLUGIN(USDVolVolumeOp, "USDVolVolumeOp", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterUsdType<UsdVolVolume>(
        "USDVolVolumeOp");

    USD_OP_REGISTER_PLUGIN(
        UsdArnold_LocationDecorator, "UsdArnold_LocationDecorator", 0, 1);
    PxrUsdKatanaUsdInPluginRegistry::RegisterLocationDecoratorOp(
        "UsdArnold_LocationDecorator");
}
