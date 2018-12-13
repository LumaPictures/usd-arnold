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
/*
 * TODO:
 *  - Properly parse type and array size.
 *  - Generate output types based on shader output type.
 * */
#include "pxr/usd/ndrAi/aiParser.h"

#include <pxr/base/tf/staticTokens.h>

#include <pxr/usd/ndr/node.h>

#include <pxr/usd/sdr/shaderNode.h>
#include <pxr/usd/sdr/shaderProperty.h>

#include <pxr/usd/sdf/propertySpec.h>

#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/property.h>

#include "pxr/usd/ndrAi/utils.h"

PXR_NAMESPACE_OPEN_SCOPE

NDR_REGISTER_PARSER_PLUGIN(NdrAiParserPlugin);

TF_DEFINE_PRIVATE_TOKENS(_tokens, (arnold)(binary));

NdrAiParserPlugin::NdrAiParserPlugin() {}

NdrAiParserPlugin::~NdrAiParserPlugin() {}

NdrNodeUniquePtr NdrAiParserPlugin::Parse(
    const NdrNodeDiscoveryResult& discoveryResult) {
    auto shaderDefs = NdrAiGetShaderDefs();
    auto prim = shaderDefs->GetPrimAtPath(SdfPath(
        TfStringPrintf("/%s", discoveryResult.identifier.GetText() + 3)));
    if (!prim) { return nullptr; }
    NdrPropertyUniquePtrVec properties;
    const auto props = prim.GetAuthoredProperties();
    properties.reserve(props.size());
    for (const auto& property : props) {
        const auto& propertyName = property.GetName();
        if (TfStringContains(propertyName.GetString(), ":")) { continue; }
        const auto propertyStack = property.GetPropertyStack();
        if (propertyStack.empty()) { continue; }
        const auto attr = prim.GetAttribute(propertyName);
        VtValue v;
        attr.Get(&v);
        properties.emplace_back(
            SdrShaderPropertyUniquePtr(new SdrShaderProperty(
                propertyName,                                 // name
                propertyStack[0]->GetTypeName().GetAsToken(), // type
                v,                                            // defaultValue
                false,                                        // isOutput
                0,                                            // arraySize
                NdrTokenMap(),                                // metadata
                NdrTokenMap(),                                // hints
                NdrOptionVec()                                // options
                )));
    }
    return NdrNodeUniquePtr(new SdrShaderNode(
        discoveryResult.identifier,    // identifier
        discoveryResult.version,       // version
        discoveryResult.name,          // name
        discoveryResult.family,        // family
        discoveryResult.discoveryType, // context
        discoveryResult.sourceType,    // sourceType
        discoveryResult.uri,           // uri
        std::move(properties)));
}

const NdrTokenVec& NdrAiParserPlugin::GetDiscoveryTypes() const {
    static const NdrTokenVec ret = {_tokens->arnold};
    return ret;
}

const TfToken& NdrAiParserPlugin::GetSourceType() const {
    return _tokens->arnold;
}

PXR_NAMESPACE_CLOSE_SCOPE
