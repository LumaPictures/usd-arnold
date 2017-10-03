#include "readPrim.h"

#include <pxr/usd/usdAi/aiShapeAPI.h>
#include <usdKatana/attrMap.h>

#include "arnoldHelpers.h"

PXR_NAMESPACE_OPEN_SCOPE

void readPrimLocation(
    FnKat::GeolibCookInterface& interface,
    FnKat::GroupAttribute opArgs,
    PxrUsdKatanaUsdInPrivateData* privateData) {
    // privateData can be a nullptr!
    if (privateData == nullptr) { return; }
    const auto prim = privateData->GetUsdPrim();
    if (!prim.IsValid()) { return; }
    
    // TODO: Remove the same call from readProcedural, because this function runs on the
    // procedural locations too.
    FnKat::GroupAttribute arnoldStatements = GetArnoldStatementsGroup(prim);
    if (arnoldStatements.isValid()) {
        static constexpr auto statementsName = "arnoldStatements";
        // This code is taken from PxrUsdKatanaAttrMap::toInterface
        // so we don't have to build some dynamic lists just for this.
        auto existingAttr = interface.getOutputAttr(statementsName);
        if (existingAttr.isValid()) {
            interface.setAttr(statementsName,
                              FnAttribute::GroupBuilder()
                                  .update(existingAttr)
                                  .deepUpdate(arnoldStatements)
                                  .build());
        } else {
            // if it's not a GroupAttr we need to call this
            interface.setAttr(statementsName, arnoldStatements);
        }
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
