// Copyright (c) 2019 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "readProcedural.h"

#include <usdKatana/attrMap.h>

#include <usdKatana/readXformable.h>
#include <usdKatana/usdInPrivateData.h>

#include <pxr/usd/usdAi/aiProcedural.h>
#include <pxr/usd/usdAi/aiProceduralNode.h>

#include "arnoldHelpers.h"

PXR_NAMESPACE_OPEN_SCOPE

void readAiProcedural(
    const UsdAiProcedural& procedural, const PxrUsdKatanaUsdInPrivateData& data,
    PxrUsdKatanaAttrMap& attrs) {
    // Read in general attributes for a transformable prim.
    PxrUsdKatanaReadXformable(procedural, data, attrs);

    const UsdPrim& prim = procedural.GetPrim();
    const double currentTime = data.GetUsdInArgs()->GetCurrentTime();

    attrs.set("type", FnKat::StringAttribute("renderer procedural"));

    SdfAssetPath filepath;
    if (auto fileAttr = procedural.GetFilepathAttr()) {
        if (fileAttr.HasValue()) { fileAttr.Get(&filepath, currentTime); }
    }
    attrs.set(
        "rendererProcedural.procedural",
        FnKat::StringAttribute(filepath.GetResolvedPath()));

    // This plugin is registered for both AiProcedural and AiProceduralNode,
    // and we need to handle them slightly differently for KtoA.
    if (auto proceduralNode = UsdAiProceduralNode(prim)) {
        TfToken nodeType;
        proceduralNode.GetNodeTypeAttr().Get(&nodeType);
        attrs.set(
            "rendererProcedural.node",
            FnKat::StringAttribute(nodeType.GetString()));
    } else {
        attrs.set(
            "rendererProcedural.node", FnKat::StringAttribute("procedural"));
    }

    FnKat::GroupBuilder argsBuilder;
    if (applyProceduralArgsAttrs(
            procedural.GetPrim(), argsBuilder, currentTime)) {
        argsBuilder.set(
            "__outputStyle", FnKat::StringAttribute("typedArguments"));
        attrs.set("rendererProcedural.args", argsBuilder.build());
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
