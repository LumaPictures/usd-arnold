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
#ifndef HDAI_CONFIG_H
#define HDAI_CONFIG_H

#include <pxr/pxr.h>

#include <pxr/base/tf/singleton.h>

#include "pxr/imaging/hdAi/api.h"

PXR_NAMESPACE_OPEN_SCOPE

class HdAiConfig {
public:
    HDAI_API
    static const HdAiConfig& GetInstance();

    /// HDAI_bucket_size
    int bucket_size;

    /// HDAI_abort_on_error
    bool abort_on_error;

    /// HDAI_AA_samples
    int AA_samples;

    /// HDAI_GI_diffuse_depth
    int GI_diffuse_depth;

    /// HDAI_GI_specular_depth
    int GI_specular_depth;

    /// HDAI_enable_progressive_render
    bool enable_progressive_render;

    /// HDAI_shutter_start
    float shutter_start;

    /// HDAI_shutter_end
    float shutter_end;

private:
    HDAI_API
    HdAiConfig();
    ~HdAiConfig() = default;
    HdAiConfig(const HdAiConfig&) = delete;
    HdAiConfig(HdAiConfig&&) = delete;
    HdAiConfig& operator=(const HdAiConfig&) = delete;

    friend class TfSingleton<HdAiConfig>;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_CONFIG_H
