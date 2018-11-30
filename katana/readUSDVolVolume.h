#ifndef PXRUSDKATANA_READUSDVOLVOLUME_H
#define PXRUSDKATANA_READUSDVOLVOLUME_H

#include <pxr/pxr.h>

#include <usdKatana/usdInPrivateData.h>

PXR_NAMESPACE_OPEN_SCOPE


void
readUSDVolVolume(
    FnKat::GeolibCookInterface& interface,
    FnKat::GroupAttribute opArgs,
    const PxrUsdKatanaUsdInPrivateData& privateData);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READUSDVOLVOLUME_H
