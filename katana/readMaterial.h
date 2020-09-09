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
#ifndef PXRUSDKATANA_READMATERIAL_H
#define PXRUSDKATANA_READMATERIAL_H

#include <pxr/pxr.h>

#include <pxr/usd/usdShade/material.h>

#include <FnGeolib/op/FnGeolibCookInterface.h>

PXR_NAMESPACE_OPEN_SCOPE

void readMaterial(
    UsdStageWeakPtr stage, FnKat::GeolibCookInterface& interface,
    const UsdShadeMaterial& material);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_READMATERIAL_H
