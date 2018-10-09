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
#ifndef HDAI_NODES_H
#define HDAI_NODES_H

#include <ai.h>

#include <functional>
#include <vector>

namespace HdAiNodeNames {
extern AtString camera;
extern AtString driver;
} // namespace HdAiNodeNames

namespace HdAiCamera {
extern AtString projMtx;
extern AtString frameAspect;
} // namespace HdAiCamera

void hdAiInstallNodes();
void hdAiUninstallNodes();

struct HdAiBucketData {
    HdAiBucketData() = default;
    ~HdAiBucketData() = default;
    HdAiBucketData(const HdAiBucketData&) = delete;
    HdAiBucketData(HdAiBucketData&&) = delete;
    HdAiBucketData& operator=(const HdAiBucketData&) = delete;
    std::vector<uint8_t> data;
    int dataType = AI_TYPE_RGBA;
    int xo = 0;
    int yo = 0;
    int sizeX = 0;
    int sizeY = 0;
};

void hdAiEmptyBucketQueue(const std::function<void(const HdAiBucketData*)>& f);

#endif
