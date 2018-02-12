#ifndef PXRUSDKATANA_READVOLUME_H
#define PXRUSDKATANA_READVOLUME_H

#include <pxr/pxr.h>

PXR_NAMESPACE_OPEN_SCOPE

class PxrUsdKatanaAttrMap;
class PxrUsdKatanaUsdInPrivateData;
class UsdAiVolume;

/// \brief read \p volume into \p attrs.
void
readVolume(
    const UsdAiVolume& procedural,
    const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READVOLUME_H
