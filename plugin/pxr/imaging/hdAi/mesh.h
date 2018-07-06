#ifndef HDAI_MESH_H
#define HDAI_MESH_H

#include <pxr/pxr.h>

#include <pxr/imaging/hd/mesh.h>

#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiMesh : public HdMesh {
public:
    HdAiMesh(
        const SdfPath& id,
        const SdfPath& instancerId = SdfPath());

    ~HdAiMesh() override = default;

private:
    AtNode* _mesh;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_MESH_H
