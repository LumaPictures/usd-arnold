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

AI_SHADER_NODE_EXPORT_METHODS(primvarReaderFloat2Mtd);

namespace {
enum { p_varname, p_fallback };
}

node_parameters {
    AiParameterStr("varname", "");
    AiParameterVec2("fallback", 0.0f, 0.0f);

    AiMetaDataSetBool(nentry, "", "ndrai_dont_discover", true);
}

node_initialize {}

node_update {}

node_finish {}

shader_evaluate {
    const auto varname = AiShaderEvalParamStr(p_varname);
    if (varname.empty() || !AiUDataGetVec2(varname, sg->out.VEC2())) {
        sg->out.VEC2() = AiShaderEvalParamVec2(p_fallback);
        return;
    }
}