// Copyright (c) 2019 Luma Pictures . All rights reserved.
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
