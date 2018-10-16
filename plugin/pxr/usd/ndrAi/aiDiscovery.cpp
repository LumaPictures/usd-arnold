// Copyright (c) 2018 Luma Pictures . All rights reserved.
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
#include "pxr/usd/ndrAi/aiDiscovery.h"

#include <pxr/base/tf/getenv.h>
#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/stringUtils.h>

#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>

#include "pxr/usd/usdAi/tokens.h"

#include "pxr/usd/ndrAi/utils.h"

#include <iostream>

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PRIVATE_TOKENS(_tokens, (shader)(arnold));

NDR_REGISTER_DISCOVERY_PLUGIN(NdrAiDiscoveryPlugin);

NdrAiDiscoveryPlugin::NdrAiDiscoveryPlugin() {}

NdrAiDiscoveryPlugin::~NdrAiDiscoveryPlugin() {}

NdrNodeDiscoveryResultVec NdrAiDiscoveryPlugin::DiscoverNodes(
    const Context& context) {
    NdrNodeDiscoveryResultVec ret;
    auto shaderDefs = NdrAiGetShaderDefs();
    for (const UsdPrim& prim : shaderDefs->Traverse()) {
        const auto shaderName = prim.GetName();
        TfToken filename("<built-in>");
        prim.GetMetadata(UsdAiTokens->filename, &filename);
        ret.emplace_back(
            NdrIdentifier(TfStringPrintf(
                "arnold_%i_%i_%s", AI_VERSION_ARCH_NUM, AI_VERSION_MAJOR_NUM,
                shaderName.GetText())),                            // identifier
            NdrVersion(AI_VERSION_ARCH_NUM, AI_VERSION_MAJOR_NUM), // version
            shaderName,                                            // name
            _tokens->shader,                                       // family
            _tokens->arnold, // discoveryType
            _tokens->arnold, // sourceType
            filename,        // uri
            filename         // resolvedUri
        );
    }
    return ret;
}

const NdrStringVec& NdrAiDiscoveryPlugin::GetSearchURIs() const {
    static const auto result = []() -> NdrStringVec {
        NdrStringVec ret = TfStringSplit(TfGetenv("ARNOLD_PLUGIN_PATH"), ":");
        ret.push_back("<built-in>");
        return ret;
    }();
    return result;
}

PXR_NAMESPACE_CLOSE_SCOPE
