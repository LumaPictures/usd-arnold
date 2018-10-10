#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(primvarReaderFloat2Mtd);

namespace {
enum { p_varname, p_fallback };
}

node_parameters {
    AiParameterStr("varname", "");
    AiParameterVec2("fallback", 0.0f, 0.0f);
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