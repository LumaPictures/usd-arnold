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
#include <ai.h>

#include <tbb/concurrent_queue.h>

#include <memory>

#include "pxr/imaging/hdAi/nodes/nodes.h"

AI_DRIVER_NODE_EXPORT_METHODS(HdAiDriverMtd);

namespace {
const char* supportedExtensions[] = {nullptr};
}

tbb::concurrent_queue<HdAiBucketData*> bucketQueue;

void hdAiEmptyBucketQueue(const std::function<void(const HdAiBucketData*)>& f) {
    HdAiBucketData* data = nullptr;
    while (bucketQueue.try_pop(data)) {
        if (data) {
            f(data);
            delete data;
            data = nullptr;
        }
    }
}

node_parameters {}

node_initialize { AiDriverInitialize(node, false); }

node_update {}

node_finish {}

driver_supports_pixel_type { return pixel_type == AI_TYPE_RGBA; }

driver_extension { return supportedExtensions; }

driver_open {}

driver_needs_bucket { return true; }

driver_prepare_bucket {}

driver_process_bucket {
    const char* outputName = nullptr;
    int pixelType = AI_TYPE_RGBA;
    const void* bucketData = nullptr;
    while (AiOutputIteratorGetNext(
        iterator, &outputName, &pixelType, &bucketData)) {
        if (pixelType != AI_TYPE_RGBA) { continue; }
        auto* data = new HdAiBucketData();
        data->dataType = AI_TYPE_RGBA;
        data->xo = bucket_xo;
        data->yo = bucket_yo;
        data->sizeX = bucket_size_x;
        data->sizeY = bucket_size_y;
        const size_t memSize =
            bucket_size_x * bucket_size_y * sizeof(float) * 4;
        data->data.resize(memSize, 0);
        memcpy(data->data.data(), bucketData, memSize);
        bucketQueue.push(data);
        break; // Only one buffer for now.
    }
}

driver_write_bucket {}

driver_close {}
