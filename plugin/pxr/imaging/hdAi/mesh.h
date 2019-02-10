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
#ifndef HDAI_MESH_H
#define HDAI_MESH_H

#include <pxr/pxr.h>
#include "pxr/imaging/hdAi/api.h"

#include <pxr/imaging/hd/mesh.h>

#include "pxr/imaging/hdAi/renderDelegate.h"

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiMesh : public HdMesh {
public:
    HDAI_API
    HdAiMesh(
        HdAiRenderDelegate* delegate, const SdfPath& id,
        const SdfPath& instancerId = SdfPath());

    HDAI_API
    ~HdAiMesh() override;

    HDAI_API
    void Sync(
        HdSceneDelegate* delegate, HdRenderParam* renderParam,
        HdDirtyBits* dirtyBits, const TfToken& reprToken) override;

    HDAI_API
    HdDirtyBits GetInitialDirtyBitsMask() const override;

protected:
    HDAI_API
    HdDirtyBits _PropagateDirtyBits(HdDirtyBits bits) const override;

    HDAI_API
    void _InitRepr(const TfToken& reprToken, HdDirtyBits* dirtyBits) override;

    HdAiRenderDelegate* _delegate;
    AtNode* _mesh;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_MESH_H
