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
#include "pxr/imaging/hdAi/config.h"

#include <pxr/base/tf/envSetting.h>
#include <pxr/base/tf/instantiateSingleton.h>

PXR_NAMESPACE_OPEN_SCOPE

TF_INSTANTIATE_SINGLETON(HdAiConfig);

TF_DEFINE_ENV_SETTING(HDAI_bucket_size, 24, "Bucket size.");
;

TF_DEFINE_ENV_SETTING(HDAI_abort_on_error, false, "Abort on error.");

TF_DEFINE_ENV_SETTING(HDAI_AA_samples, 5, "Number of AA samples by default.");

TF_DEFINE_ENV_SETTING(
    HDAI_GI_diffuse_depth, 1, "Diffuse ray depth by default.");

TF_DEFINE_ENV_SETTING(
    HDAI_GI_specular_depth, 1, "Diffuse ray depth by default.");

TF_DEFINE_ENV_SETTING(
    HDAI_enable_progressive_render, true, "Enable progressive render.");

// This macro doesn't support floating point values.
TF_DEFINE_ENV_SETTING(
    HDAI_shutter_start, "-0.25f", "Shutter start for the camera.");

TF_DEFINE_ENV_SETTING(HDAI_shutter_end, "0.25f", "Shutter end for the camera.");

HdAiConfig::HdAiConfig() {
    bucket_size = std::max(1, TfGetEnvSetting(HDAI_bucket_size));
    abort_on_error = TfGetEnvSetting(HDAI_abort_on_error);
    AA_samples = std::max(1, TfGetEnvSetting(HDAI_AA_samples));
    GI_diffuse_depth = std::max(0, TfGetEnvSetting(HDAI_GI_diffuse_depth));
    GI_specular_depth = std::max(0, TfGetEnvSetting(HDAI_GI_specular_depth));
    enable_progressive_render = TfGetEnvSetting(HDAI_enable_progressive_render);
    shutter_start = static_cast<float>(
        std::atof(TfGetEnvSetting(HDAI_shutter_start).c_str()));
    shutter_end = static_cast<float>(
        std::atof(TfGetEnvSetting(HDAI_shutter_end).c_str()));
}

const HdAiConfig& HdAiConfig::GetInstance() {
    return TfSingleton<HdAiConfig>::GetInstance();
}

PXR_NAMESPACE_CLOSE_SCOPE
