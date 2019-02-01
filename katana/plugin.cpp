// Copyright (c) 2019 Luma Pictures . All rights reserved.
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
