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
#include "pxr/imaging/hdAi/nodes/nodes.h"

#include <array>
#include <tuple>

extern AtNodeMethods* HdAiDriverMtd;

AtString HdAiNodeNames::driver("HdAiDriver");

namespace {
struct NodeDefinition {
    int type;
    uint8_t outputType;
    AtString& name;
    const AtNodeMethods* methods;
};

using BuiltInNodes = std::vector<NodeDefinition>;

const auto builtInNodes = []() -> const BuiltInNodes& {
    static const BuiltInNodes ret{
        {AI_NODE_DRIVER, AI_TYPE_UNDEFINED, HdAiNodeNames::driver,
         HdAiDriverMtd},
    };
    return ret;
};

} // namespace

void hdAiInstallNodes() {
    for (const auto& it : builtInNodes()) {
        AiNodeEntryInstall(
            it.type, it.outputType, it.name, "<built-in>", it.methods,
            AI_VERSION);
    }
}

void hdAiUninstallNodes() {
    for (const auto& it : builtInNodes()) { AiNodeEntryUninstall(it.name); }
}
