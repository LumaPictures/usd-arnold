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
#include "pxr/imaging/hdAi/utils.h"

PXR_NAMESPACE_USING_DIRECTIVE

AI_DRIVER_NODE_EXPORT_METHODS(HdAiDriverMtd);

AtString HdAiDriver::projMtx("projMtx");
AtString HdAiDriver::viewMtx("viewMtx");

namespace {
const char* supportedExtensions[] = {nullptr};

struct DriverData {
    // I think we just uncovered a bug in Arnold.
    // This fails with AtMatrix, looks like as if the [3][3] of the proj
    // matrix is used incorrectly.
    GfMatrix4f projMtx;
    GfMatrix4f viewMtx;
};
} // namespace

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

node_parameters {
    AiParameterMtx(HdAiDriver::projMtx, AiM4Identity());
    AiParameterMtx(HdAiDriver::viewMtx, AiM4Identity());
}

node_initialize {
    AiDriverInitialize(node, true);
    AiNodeSetLocalData(node, new DriverData());
}

#include <iostream>

node_update {
    auto* data = reinterpret_cast<DriverData*>(AiNodeGetLocalData(node));
    data->projMtx =
        HdAiConvertMatrix(AiNodeGetMatrix(node, HdAiDriver::projMtx));
    data->viewMtx =
        HdAiConvertMatrix(AiNodeGetMatrix(node, HdAiDriver::viewMtx));
}

node_finish {}

driver_supports_pixel_type {
    return pixel_type == AI_TYPE_RGBA || pixel_type == AI_TYPE_VECTOR;
}

driver_extension { return supportedExtensions; }

driver_open {}

driver_needs_bucket { return true; }

driver_prepare_bucket {}

driver_process_bucket {
    const auto* driverData =
        reinterpret_cast<const DriverData*>(AiNodeGetLocalData(node));
    const char* outputName = nullptr;
    int pixelType = AI_TYPE_RGBA;
    const void* bucketData = nullptr;
    auto* data = new HdAiBucketData();
    data->xo = bucket_xo;
    data->yo = bucket_yo;
    data->sizeX = bucket_size_x;
    data->sizeY = bucket_size_y;
    const auto bucketSize = bucket_size_x * bucket_size_y;
    while (AiOutputIteratorGetNext(
        iterator, &outputName, &pixelType, &bucketData)) {
        if (pixelType == AI_TYPE_RGBA && strcmp(outputName, "RGBA") == 0) {
            data->beauty.resize(bucketSize);
            const auto* inRGBA = reinterpret_cast<const AtRGBA*>(bucketData);
            for (auto i = decltype(bucketSize){0}; i < bucketSize; ++i) {
                const auto in = inRGBA[i];
                const auto x = bucket_xo + i % bucket_size_x;
                const auto y = bucket_yo + i / bucket_size_x;
                AtRGBA8 out;
                out.r = AiQuantize8bit(x, y, 0, in.r, true);
                out.g = AiQuantize8bit(x, y, 1, in.g, true);
                out.b = AiQuantize8bit(x, y, 2, in.b, true);
                out.a = AiQuantize8bit(x, y, 3, in.a, true);
                data->beauty[i] = out;
            }

        } else if (
            pixelType == AI_TYPE_VECTOR && strcmp(outputName, "P") == 0) {
            data->depth.resize(bucketSize, 1.0f);
            const auto* pp = reinterpret_cast<const GfVec3f*>(bucketData);
            auto* pz = data->depth.data();
            for (auto i = decltype(bucketSize){0}; i < bucketSize; ++i) {
                // Rays hitting the background will return a (0,0,0) vector.
                const auto p = driverData->projMtx.Transform(
                    driverData->viewMtx.Transform(pp[i]));
                pz[i] = std::max(-1.0f, std::min(1.0f, p[2]));
            }
        }
    }
    if (data->beauty.empty() || data->depth.empty()) {
        delete data;
    } else {
        for (auto i = decltype(bucketSize){0}; i < bucketSize; ++i) {
            if (data->beauty[i].a == 0) { data->depth[i] = 1.0f; }
        }
        bucketQueue.push(data);
    }
}

driver_write_bucket {}

driver_close {}
