#ifndef PXRUSDKATANA_READVOLUME_H
#define PXRUSDKATANA_READVOLUME_H

#include <pxr/pxr.h>

#include <usdKatana/usdInPrivateData.h>

PXR_NAMESPACE_OPEN_SCOPE

class PxrUsdKatanaUsdInPrivateData;


void
readVolume(
    FnKat::GeolibCookInterface& interface,
    FnKat::GroupAttribute opArgs,
    const PxrUsdKatanaUsdInPrivateData& privateData);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READVOLUME_H
