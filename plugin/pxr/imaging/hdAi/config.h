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
#ifndef HDAI_CONFIG_H
#define HDAI_CONFIG_H

#include <pxr/base/tf/singleton.h>
#include <pxr/pxr.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiConfig {
public:
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
    HdAiConfig();
    ~HdAiConfig() = default;
    HdAiConfig(const HdAiConfig&) = delete;
    HdAiConfig(HdAiConfig&&) = delete;
    HdAiConfig& operator=(const HdAiConfig&) = delete;

    friend class TfSingleton<HdAiConfig>;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_CONFIG_H
