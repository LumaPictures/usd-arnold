#include "pxr/imaging/hdAi/mesh.h"

PXR_NAMESPACE_OPEN_SCOPE

HdAiMesh::HdAiMesh(
    const SdfPath& id,
    const SdfPath& instancerId)
    : HdMesh(id, instancerId) {
    _mesh = AiNode("polymesh");
    AiNodeSetStr(_mesh, "name", id.GetText());
}

PXR_NAMESPACE_CLOSE_SCOPE
