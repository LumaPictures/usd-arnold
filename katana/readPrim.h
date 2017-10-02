#ifndef PXRUSDKATANA_READPRIM_H
#define PXRUSDKATANA_READPRIM_H

#include <pxr/pxr.h>

#include <usdKatana/usdInPluginRegistry.h>

PXR_NAMESPACE_OPEN_SCOPE

void readPrimLocation(
    FnKat::GeolibCookInterface& interface,
    FnKat::GroupAttribute opArgs,
    PxrUsdKatanaUsdInPrivateData* privateData);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READPRIM_H
