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
#ifndef HDAI_OPENVDB_ASSET_H
#define HDAI_OPENVDB_ASSET_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/field.h>

#include "pxr/imaging/hdAi/renderDelegate.h"

#include <mutex>
#include <unordered_set>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiOpenvdbAsset : public HdField {
public:
    HDAI_API
    HdAiOpenvdbAsset(HdAiRenderDelegate* delegate, const SdfPath& id);

    HDAI_API
    void Sync(
        HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
        HdDirtyBits* dirtyBits) override;

    HDAI_API
    HdDirtyBits GetInitialDirtyBitsMask() const override;

    HDAI_API
    void TrackVolumePrimitive(const SdfPath& id);

private:
    // We are creating the arnold prims via HdAiVolume primitive, not
    // the HdField class. So when the file path has changed,
    // we need to trigger the update of the volume primitive.
    // Rprims can be synced on multiple threads, so we need a simple mutex
    // to store the affected volume prims.
    std::mutex _volumeListMutex;
    std::unordered_set<SdfPath, SdfPath::Hash> _volumeList;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_OPENVDB_ASSET_H
