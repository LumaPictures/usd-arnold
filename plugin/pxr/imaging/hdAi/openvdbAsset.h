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
