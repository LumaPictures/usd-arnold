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
// This is just to suppress a warning message from Houdini.
// The plugin works without this.
#include <SYS/SYS_Version.h>
#include <UT/UT_DSOVersion.h>
#include <OP/OP_OperatorTable.h>

extern "C" {
    // An empty function so houdini won't complain about missing symbols.
    void newDriverOperator(OP_OperatorTable* operators) 
    {
    }
}

#include <gusd/shadingModeRegistry.h>
#include <gusd/primWrapper.h>

#include "arnoldShadingExporter.h"

PXR_NAMESPACE_USING_DIRECTIVE

TF_REGISTRY_FUNCTION_WITH_TAG(GusdShadingModeRegistry, arnold) {
    GusdShadingModeRegistry::getInstance().registerExporter(
        "arnold", "Arnold", &exportArnoldShaders);
}
