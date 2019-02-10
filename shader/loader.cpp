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
#include <ai.h>
#include <cstdio>

#include <vector>

extern AtNodeMethods* previewSurfaceMtd;
extern AtNodeMethods* primvarReaderFloat2Mtd;
extern AtNodeMethods* uvTextureMtd;

node_loader {
    struct NodeDef {
        AtNodeMethods* methods;
        uint8_t outputType;
        const char* name;
        int nodeType;
    };

    const static std::vector<NodeDef> nodes{
        {previewSurfaceMtd, AI_TYPE_CLOSURE, "UsdPreviewSurface",
         AI_NODE_SHADER},
        {primvarReaderFloat2Mtd, AI_TYPE_VECTOR2, "UsdPrimvarReader_float2",
         AI_NODE_SHADER},
        {uvTextureMtd, AI_TYPE_RGBA, "UsdUVTexture", AI_NODE_SHADER}};

    const auto is = static_cast<size_t>(i);
    if (is >= nodes.size()) {
        return false;
    } else {
        const auto& n = nodes[is];
        node->methods = n.methods;
        node->output_type = n.outputType;
        node->name = n.name;
        node->node_type = n.nodeType;
        sprintf(node->version, AI_VERSION);
        return true;
    }
}
