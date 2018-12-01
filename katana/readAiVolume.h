#ifndef PXRUSDKATANA_READAIVOLUME_H
#define PXRUSDKATANA_READAIVOLUME_H

#include <pxr/pxr.h>

#include <usdKatana/usdInPrivateData.h>

PXR_NAMESPACE_OPEN_SCOPE


void
readAiVolume(
    FnKat::GeolibCookInterface& interface,
    FnKat::GroupAttribute opArgs,
    const PxrUsdKatanaUsdInPrivateData& privateData);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READAIVOLUME_H
