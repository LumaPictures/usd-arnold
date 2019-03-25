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
#ifndef HDAI_NODES_H
#define HDAI_NODES_H

#include <ai.h>

#include <functional>
#include <vector>

namespace HdAiNodeNames {
extern AtString driver;
} // namespace HdAiNodeNames

namespace HdAiDriver {
extern AtString projMtx;
extern AtString viewMtx;
} // namespace HdAiDriver

void hdAiInstallNodes();
void hdAiUninstallNodes();

struct AtRGBA8 {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

struct HdAiBucketData {
    HdAiBucketData() = default;
    ~HdAiBucketData() = default;
    HdAiBucketData(const HdAiBucketData&) = delete;
    HdAiBucketData(HdAiBucketData&&) = delete;
    HdAiBucketData& operator=(const HdAiBucketData&) = delete;
    int xo = 0;
    int yo = 0;
    int sizeX = 0;
    int sizeY = 0;
    std::vector<AtRGBA8> beauty;
    std::vector<float> depth;
};

void hdAiEmptyBucketQueue(const std::function<void(const HdAiBucketData*)>& f);

#endif
