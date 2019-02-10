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
#include "pxr/usd/usdAi/utils.h"

#include <array>

PXR_NAMESPACE_OPEN_SCOPE

UsdStageRefPtr UsdAiGetArnoldShaderDesc(const std::string& additionalFlags) {
    UsdStageRefPtr ret = nullptr;
    std::stringstream command;
    command << "usdAiShaderInfo --cout";
    if (!additionalFlags.empty()) { command << " " << additionalFlags; }
    FILE* pipe = popen(command.str().c_str(), "r");
    if (pipe != nullptr) {
        std::stringstream result;
        std::array<char, 4096> buffer = {0};
        while (!feof(pipe)) {
            if (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                result << buffer.data();
            }
        }
        pclose(pipe);
        ret = UsdStage::CreateInMemory(".usda");
        ret->GetRootLayer()->ImportFromString(result.str());
    }
    return ret;
}

PXR_NAMESPACE_CLOSE_SCOPE
