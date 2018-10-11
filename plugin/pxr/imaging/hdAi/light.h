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
#ifndef HDAI_LIGHT_H
#define HDAI_LIGHT_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/light.h>

#include "pxr/imaging/hdAi/renderDelegate.h"

#include <functional>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiLight : public HdLight {
public:
    static HdAiLight* CreatePointLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    static HdAiLight* CreateDistantLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    static HdAiLight* CreateDiskLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    static HdAiLight* CreateRectLight(
        HdAiRenderDelegate* delegate, const SdfPath& id);

    void Sync(
        HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
        HdDirtyBits* dirtyBits) override;

    HdDirtyBits GetInitialDirtyBitsMask() const override;

protected:
    using SyncParams = std::function<void(
        AtNode*, const AtNodeEntry*, const SdfPath&, HdSceneDelegate*)>;
    HdAiLight(
        HdAiRenderDelegate* delegate, const SdfPath& id,
        const AtString& arnoldType, const SyncParams& sync);

    SyncParams _syncParams;
    HdAiRenderDelegate* _delegate;
    AtNode* _light;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_LIGHT_H
