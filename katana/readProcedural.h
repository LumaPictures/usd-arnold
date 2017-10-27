#ifndef PXRUSDKATANA_READAIPROCEDURAL_H
#define PXRUSDKATANA_READAIPROCEDURAL_H

#include <pxr/pxr.h>

PXR_NAMESPACE_OPEN_SCOPE

class PxrUsdKatanaAttrMap;
class PxrUsdKatanaUsdInPrivateData;
class UsdAiProcedural;

/// \brief read \p procedural into \p attrs.
void
readAiProcedural(
    const UsdAiProcedural& procedural,
    const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READAIPROCEDURAL_H