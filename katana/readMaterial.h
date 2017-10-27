#ifndef PXRUSDKATANA_READMATERIAL_H
#define PXRUSDKATANA_READMATERIAL_H

#include <pxr/pxr.h>

#include <pxr/usd/usdAi/aiMaterialAPI.h>

#include <FnGeolib/op/FnGeolibCookInterface.h>

PXR_NAMESPACE_OPEN_SCOPE

void readMaterial(
    UsdStageWeakPtr stage,
    FnKat::GeolibCookInterface& interface,
    const UsdAiMaterialAPI& material);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READMATERIAL_H
