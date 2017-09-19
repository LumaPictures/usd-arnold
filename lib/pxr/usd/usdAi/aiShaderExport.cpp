#include "pxr/usd/usdAi/aiShaderExport.h"

#include "pxr/base/gf/matrix4f.h"
#include "pxr/usd/sdf/types.h"
#include "pxr/usd/usdAi/aiMaterialAPI.h"
#include "pxr/usd/usdAi/aiNodeAPI.h"
#include "pxr/usd/usd/relationship.h"
#include "pxr/usd/usd/primRange.h"
#include "pxr/usd/usdGeom/scope.h"
#include "pxr/usd/usdGeom/xform.h"
#include "pxr/usd/usdShade/tokens.h"
#include "pxr/usd/usdShade/material.h"
#include "pxr/usd/usdShade/input.h"
#include "pxr/usd/usdShade/connectableAPI.h"

#include <tbb/tick_count.h>


PXR_NAMESPACE_OPEN_SCOPE

namespace {

    inline GfMatrix4f NodeGetMatrix(const AtNode* node, const char* param) {
        AtMatrix mat;
        AiNodeGetMatrix(node, param, mat);
        return GfMatrix4f(mat);
    };

    inline GfMatrix4f ArrayGetMatrix(const AtArray* arr, uint32_t i, const char* file, uint32_t line) {
        AtMatrix mat;
        AiArrayGetMtxFunc(arr, i, mat, file, line);
        return GfMatrix4f(mat);
    }

    inline const char* GetEnum(AtEnum en, int32_t id) {
        if (en == nullptr) { return ""; }
        if (id < 0) { return ""; }
        for (auto i = 0; i <= id; ++i) {
            if (en[i] == nullptr) {
                return "";
            }
        }
        return en[id];
    }

    template <typename LHT, typename RHT> inline
    void convert(LHT& l, const RHT& r) {
        static_assert(sizeof(LHT) == sizeof(RHT), "Input data for convert must has the same size");
        memcpy(&l, &r, sizeof(r));
    };

    template <> inline
    void convert<std::string, const char*>(std::string& l, const char* const& r) {
        l = r;
    };

    template <typename T, typename R = T> inline
    void export_array(UsdShadeInput& param, const AtArray* arr, R (*f) (const AtArray*, uint32_t, const char*, int32_t)) {
        // we already check the validity of the array before this call
        VtArray<T> out_arr(arr->nelements);
        for (auto i = 0u; i < arr->nelements; ++i) {
            convert(out_arr[i], f(arr, i, __FILE__, __LINE__));
        }
        param.Set(VtValue(out_arr));
    }

    struct simple_type {
        const SdfValueTypeName& type;
        std::function<VtValue(const AtNode*, const char*)> f;

        simple_type(const SdfValueTypeName& _type, std::function<VtValue(const AtNode*, const char*)> _f) :
            type(_type), f(_f) { }
    };

    // TODO: use sorted vectors with lower bound searches. Too many ptr jumps here
    const std::map<uint8_t, simple_type> simple_type_map = {
        {AI_TYPE_BYTE, {SdfValueTypeNames->UChar, [](const AtNode* no, const char* na) -> VtValue { return VtValue(AiNodeGetByte(no, na)); }}},
        {AI_TYPE_INT, {SdfValueTypeNames->Int, [](const AtNode* no, const char* na) -> VtValue { return VtValue(AiNodeGetInt(no, na)); }}},
        {AI_TYPE_UINT, {SdfValueTypeNames->UInt, [](const AtNode* no, const char* na) -> VtValue { return VtValue(AiNodeGetUInt(no, na)); }}},
        {AI_TYPE_BOOLEAN, {SdfValueTypeNames->Bool, [](const AtNode* no, const char* na) -> VtValue { return VtValue(AiNodeGetBool(no, na)); }}},
        {AI_TYPE_FLOAT, {SdfValueTypeNames->Float, [](const AtNode* no, const char* na) -> VtValue { return VtValue(AiNodeGetFlt(no, na)); }}},
        {AI_TYPE_RGB, {SdfValueTypeNames->Color3f, [](const AtNode* no, const char* na) -> VtValue { const auto v = AiNodeGetRGB(no, na); return VtValue(GfVec3f(v.r, v.g, v.b)); }}},
        {AI_TYPE_RGBA, {SdfValueTypeNames->Color4f, [](const AtNode* no, const char* na) -> VtValue { const auto v = AiNodeGetRGBA(no, na); return VtValue(GfVec4f(v.r, v.g, v.b, v.a)); }}},
        {AI_TYPE_VECTOR, {SdfValueTypeNames->Vector3f, [](const AtNode* no, const char* na) -> VtValue { const auto v = AiNodeGetVec(no, na); return VtValue(GfVec3f(v.x, v.y, v.z)); }}},
        {AI_TYPE_POINT, {SdfValueTypeNames->Vector3f, [](const AtNode* no, const char* na) -> VtValue { const auto v = AiNodeGetPnt(no, na); return VtValue(GfVec3f(v.x, v.y, v.z)); }}},
        {AI_TYPE_POINT2, {SdfValueTypeNames->Float2, [](const AtNode* no, const char* na) -> VtValue { const auto v = AiNodeGetPnt2(no, na); return VtValue(GfVec2f(v.x, v.y)); }}},
        {AI_TYPE_STRING, {SdfValueTypeNames->String, [](const AtNode* no, const char* na) -> VtValue { return VtValue(AiNodeGetStr(no, na)); }}},
        {AI_TYPE_NODE, {SdfValueTypeNames->String, nullptr}},
        {AI_TYPE_MATRIX, {SdfValueTypeNames->Matrix4d, [](const AtNode* no, const char* na) -> VtValue { return VtValue(NodeGetMatrix(no, na)); }}},
        {AI_TYPE_ENUM, {SdfValueTypeNames->String, [](const AtNode* no, const char* na) -> VtValue {
            const auto* nentry = AiNodeGetNodeEntry(no);
            if (nentry == nullptr) { return VtValue(""); }
            const auto* pentry = AiNodeEntryLookUpParameter(nentry, na);
            if (pentry == nullptr) { return VtValue(""); }
            const auto enums = AiParamGetEnum(pentry);
            return VtValue(GetEnum(enums, AiNodeGetInt(no, na)));
        }}},
    };

    const simple_type*
    get_simple_type(uint8_t type) {
        const auto it = simple_type_map.find(type);
        if (it != simple_type_map.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    }

    struct array_type {
        const SdfValueTypeName& type;
        std::function<void(UsdShadeInput&, const AtArray*)> f;

        array_type(const SdfValueTypeName& _type, std::function<void(UsdShadeInput&, const AtArray*)> _f) :
            type(_type), f(_f) { }
    };

    const std::map<uint8_t, array_type> array_type_map = {
        {AI_TYPE_BYTE, {SdfValueTypeNames->UCharArray, [](UsdShadeInput& p, const AtArray* a) { export_array<uint8_t>(p, a, AiArrayGetByteFunc); }}},
        {AI_TYPE_INT, {SdfValueTypeNames->IntArray, [](UsdShadeInput& p, const AtArray* a) { export_array<int32_t>(p, a, AiArrayGetIntFunc); }}},
        {AI_TYPE_UINT, {SdfValueTypeNames->UIntArray, [](UsdShadeInput& p, const AtArray* a) { export_array<uint32_t>(p, a, AiArrayGetUIntFunc); }}},
        {AI_TYPE_BOOLEAN, {SdfValueTypeNames->BoolArray, [](UsdShadeInput& p, const AtArray* a) { export_array<bool>(p, a, AiArrayGetBoolFunc); }}},
        {AI_TYPE_FLOAT, {SdfValueTypeNames->FloatArray, [](UsdShadeInput& p, const AtArray* a) { export_array<float>(p, a, AiArrayGetFltFunc); }}},
        {AI_TYPE_RGB, {SdfValueTypeNames->Color3fArray, [](UsdShadeInput& p, const AtArray* a) { export_array<GfVec3f, AtRGB>(p, a, AiArrayGetRGBFunc); }}},
        {AI_TYPE_RGBA, {SdfValueTypeNames->Color4fArray, [](UsdShadeInput& p, const AtArray* a) { export_array<GfVec4f, AtRGBA>(p, a, AiArrayGetRGBAFunc); }}},
        {AI_TYPE_VECTOR, {SdfValueTypeNames->Vector3fArray, [](UsdShadeInput& p, const AtArray* a) { export_array<GfVec3f, AtVector>(p, a, AiArrayGetVecFunc); }}},
        {AI_TYPE_POINT, {SdfValueTypeNames->Vector3fArray, [](UsdShadeInput& p, const AtArray* a) { export_array<GfVec3f, AtPoint>(p, a, AiArrayGetPntFunc); }}},
        {AI_TYPE_POINT2, {SdfValueTypeNames->Float2Array, [](UsdShadeInput& p, const AtArray* a) { export_array<GfVec2f, AtPoint2>(p, a, AiArrayGetPnt2Func); }}},
        {AI_TYPE_STRING, {SdfValueTypeNames->StringArray, [](UsdShadeInput& p, const AtArray* a) { export_array<std::string, const char*>(p, a, AiArrayGetStrFunc); }}},
        {AI_TYPE_NODE, {SdfValueTypeNames->StringArray, nullptr}},
        {AI_TYPE_MATRIX,
            {SdfValueTypeNames->Matrix4dArray,
                           [](UsdShadeInput& p, const AtArray* a) {
                               VtArray<GfMatrix4d> arr(a->nelements);
                               for (auto i = 0u; i < a->nelements; ++i) {
                                   arr[i] = GfMatrix4d(ArrayGetMatrix(a, i, __FILE__, __LINE__));
                               }
                           }
                       }}, // TODO: implement
        {AI_TYPE_ENUM, {SdfValueTypeNames->IntArray, [](UsdShadeInput& p, const AtArray* a) { export_array<int32_t>(p, a, AiArrayGetIntFunc); }}},
    };

    const array_type*
    get_array_type(uint8_t type) {
        const auto it = array_type_map.find(type);
        if (it != array_type_map.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    }

    struct out_comp_t {
        TfToken n;
        const SdfValueTypeName& t;

        out_comp_t(const char* _n, const SdfValueTypeName& _t) : n(_n), t(_t) { }
    };
    // Preferring std::vector here over pointers, so we can get some extra
    // boundary checks in debug mode.
    const out_comp_t out_comp_name(int32_t output_type, int32_t index) {
        const static out_comp_t node_comp_name = {"node", SdfValueTypeNames->String};
        const static std::vector<out_comp_t> rgba_comp_names = {
            {"r", SdfValueTypeNames->Float},
            {"g", SdfValueTypeNames->Float},
            {"b", SdfValueTypeNames->Float},
            {"a", SdfValueTypeNames->Float},
        };
        const static std::vector<out_comp_t> vec_comp_names = {
            {"x", SdfValueTypeNames->Float},
            {"y", SdfValueTypeNames->Float},
            {"z", SdfValueTypeNames->Float},
        };
        if (index == -1) {
            auto itype = get_simple_type(static_cast<uint8_t>(output_type));
            if (itype == nullptr) {
                return node_comp_name;
            } else {
                return {"out", itype->type};
            }
        } else {
            if (output_type == AI_TYPE_RGBA || output_type == AI_TYPE_RGB) {
                return rgba_comp_names[std::min(static_cast<size_t>(index), rgba_comp_names.size() - 1)];
            } else if (output_type == AI_TYPE_VECTOR || output_type == AI_TYPE_POINT ||
                       output_type == AI_TYPE_POINT2) {
                return vec_comp_names[std::min(static_cast<size_t>(index), vec_comp_names.size() - 1)];
            } else {
                return node_comp_name;
            }
        }
    }

    using in_comp_names_t = std::vector<const char*>;
    const in_comp_names_t& in_comp_names(int32_t input_type) {
        const static in_comp_names_t empty;
        const static in_comp_names_t rgb_names {
            "r", "g", "b"
        };
        const static in_comp_names_t rgba_names = {
            "r", "g", "b", "a"
        };
        const static in_comp_names_t vec_names = {
            "x", "y", "z"
        };
        const static in_comp_names_t vec2_names = {
            "x", "y"
        };
        if (input_type == AI_TYPE_RGB) {
            return rgb_names;
        } else if (input_type == AI_TYPE_RGBA) {
            return rgba_names;
        } else if (input_type == AI_TYPE_POINT || input_type == AI_TYPE_VECTOR) {
            return vec_names;
        } else if (input_type == AI_TYPE_POINT2) {
            return vec2_names;
        } else {
            return empty;
        }
    }
}

AiShaderExport::AiShaderExport(const UsdStagePtr& _stage, const UsdTimeCode& _time_code,
                               const std::string& parent_scope) :
    m_stage(_stage),
    m_shaders_scope(parent_scope.empty() ? "/Looks" : (parent_scope + "/Looks")),
    m_time_code(_time_code)
{
    auto scope = UsdGeomScope::Define(m_stage, m_shaders_scope);
}

AiShaderExport::~AiShaderExport() {}

void AiShaderExport::clean_arnold_name(std::string& name) {
    std::replace(name.begin(), name.end(), '@', '_');
    std::replace(name.begin(), name.end(), '.', '_');
    std::replace(name.begin(), name.end(), '|', '_');
    std::replace(name.begin(), name.end(), ':', '_');
}


bool
AiShaderExport::get_output(const AtNode* src_arnold_node, UsdAiShader& src_shader,
                           UsdShadeOutput& out, bool is_node_type, int32_t src_comp_index) {

    UsdShadeConnectableAPI linked_API(src_shader);
    const auto linked_output_type = is_node_type ?
                                    AI_TYPE_NODE :
                                    AiNodeEntryGetOutputType(AiNodeGetNodeEntry(src_arnold_node));
    const auto& out_comp = out_comp_name(linked_output_type, src_comp_index);
    out = linked_API.GetOutput(out_comp.n);
    if (!out) {
        out = linked_API.CreateOutput(out_comp.n, out_comp.t);
    }

    return true;
}

bool
AiShaderExport::export_connection(const AtNode* dest_arnold_node, UsdAiShader& dest_shader,
                                  const char* dest_param_name, uint8_t arnold_param_type) {
    const auto iter_type = get_simple_type(arnold_param_type);
    if (iter_type == nullptr) {
        return false;
    }
    auto _get_output_parameter = [this, dest_arnold_node, arnold_param_type] (const char* param_name, UsdShadeOutput& out) -> bool {
        int32_t comp = -1;
        const auto src_arnold_node = arnold_param_type == AI_TYPE_NODE ?
                                 reinterpret_cast<AtNode*>(AiNodeGetPtr(dest_arnold_node, param_name)) :
                                 AiNodeGetLink(dest_arnold_node, param_name, &comp);
        if (src_arnold_node != nullptr) {
            const auto src_path = export_arnold_node(src_arnold_node, m_shaders_scope);
            auto src_shader = UsdAiShader::Get(m_stage, src_path);
            // FIXME: check for invalid src_shader
            return this->get_output(src_arnold_node, src_shader, out, arnold_param_type == AI_TYPE_NODE, comp);
        } else {
            return false;
        }
    };

    UsdShadeConnectableAPI connectable_API(dest_shader);
    UsdShadeOutput source_param;
    if (_get_output_parameter(dest_param_name, source_param)) {
        // connected
        UsdShadeConnectableAPI::ConnectToSource(dest_shader.CreateInput(TfToken(dest_param_name), iter_type->type), source_param);
        return true;
    } else {
        // not connected or failed to export node
        if (iter_type->f != nullptr) {
            auto param = dest_shader.CreateInput(TfToken(dest_param_name), iter_type->type);
            param.Set(iter_type->f(dest_arnold_node, dest_param_name));
            return true;
        }
    }

    /*for (const auto& comps : in_comp_names(arnold_param_type)) {
        const auto arnold_comp_name = std::string(dest_param_name) + "." + comps;
        const auto usd_comp_name = std::string(dest_param_name) + ":" + comps;
        if (_get_output_parameter(arnold_comp_name.c_str(), source_param)) {
            auto param_comp = connectable_API.CreateInput(TfToken(usd_comp_name),
                                                          SdfValueTypeNames->Float);
            if (param_comp) {
                connectable_API.ConnectToSource(param_comp, source_param);
            }
        }
    }*/
    return false;
}


bool
AiShaderExport::export_connection(const AtNode* dest_arnold_node, UsdAiShader& dest_shader,
                                  const char* dest_param_name,
                                  const AtNode* src_arnold_node, UsdAiShader& src_shader,
                                  int32_t src_comp_index) {
    const auto pentry = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(dest_arnold_node), dest_param_name);
    const auto ptype = static_cast<uint8_t>(AiParamGetType(pentry));
    return export_connection(dest_arnold_node, dest_shader,
                             dest_param_name, ptype,
                             src_arnold_node, src_shader, src_comp_index);
}

bool
AiShaderExport::export_connection(const AtNode* dest_arnold_node, UsdAiShader& dest_shader,
                                  const char* dest_param_name, uint8_t arnold_param_type,
                                  const AtNode* src_arnold_node, UsdAiShader& src_shader,
                                  int32_t src_comp_index) {
    const auto iter_type = get_simple_type(arnold_param_type);
    if (iter_type == nullptr) {
        std::cout << "unsupported type " << arnold_param_type << std::endl;
        return false;
    }

    UsdShadeOutput out;
    get_output(src_arnold_node, src_shader, out, false, src_comp_index);

    // we could assume the dest has the same type as source_param
    UsdShadeConnectableAPI::ConnectToSource(
        dest_shader.CreateInput(TfToken(dest_param_name), iter_type->type), out);
    return true;
}

void
AiShaderExport::export_parameter(
    const AtNode* arnold_node, UsdAiShader& shader, const char* arnold_param_name, uint8_t arnold_param_type, bool user) {
    if (arnold_param_type == AI_TYPE_ARRAY) {
        const auto arr = AiNodeGetArray(arnold_node, arnold_param_name);
        if (arr == nullptr || arr->nelements == 0 || arr->nkeys == 0 || arr->type == AI_TYPE_ARRAY) { return; }
        const auto iter_type = get_array_type(arr->type);
        if (iter_type == nullptr) { return; }
        auto param = shader.CreateInput(TfToken(arnold_param_name), iter_type->type);
        if (iter_type->f != nullptr) {
            iter_type->f(param, arr);
        }
    } else {
        if (user) {
            const auto iter_type = get_simple_type(arnold_param_type);
            if (iter_type == nullptr) {
                return;
            }
            UsdAiNodeAPI api(shader.GetPrim());
            auto param = api.CreateUserAttribute(TfToken(arnold_param_name), iter_type->type);
            param.Set(iter_type->f(arnold_node, arnold_param_name));
        } else {

            if (AiNodeIsLinked(arnold_node, arnold_param_name)) {
                // FIXME: should we also end up here if (arnold_param_type == AI_TYPE_NODE)??
                export_connection(arnold_node, shader, arnold_param_name, arnold_param_type);
            } else {
                const auto iter_type = get_simple_type(arnold_param_type);
                if (iter_type == nullptr  || iter_type->f == nullptr) {
                    return;
                }
                auto param = shader.CreateInput(TfToken(arnold_param_name), iter_type->type);
                param.Set(iter_type->f(arnold_node, arnold_param_name));
            }
        }
    }
}

SdfPath
AiShaderExport::export_arnold_node(const AtNode* arnold_node, SdfPath parent_path) {
    if (arnold_node == nullptr) {
        return SdfPath();
    }
    const auto nentry = AiNodeGetNodeEntry(arnold_node);
    if (AiNodeEntryGetType(nentry) != AI_NODE_SHADER) {
        return SdfPath();
    } else {
        const auto it = m_shader_to_usd_path.find(arnold_node);
        if (it != m_shader_to_usd_path.end()) {
            return it->second;
        } else {
            std::string node_name(AiNodeGetName(arnold_node));
            if (node_name == "") {
                // TODO: raise error
                return SdfPath();
            }
            // MtoA exports sub shaders with @ prefix, which is used for something else in USD
            // TODO: implement a proper cleanup using boost::regex
            clean_arnold_name(node_name);
            auto shader_path = parent_path.AppendPath(SdfPath(node_name));
            auto shader = UsdAiShader::Define(m_stage, shader_path);
            m_shader_to_usd_path.insert(std::make_pair(arnold_node, shader_path));

            shader.CreateIdAttr(VtValue(TfToken(AiNodeEntryGetName(nentry))));
            auto piter = AiNodeEntryGetParamIterator(nentry);
            while (!AiParamIteratorFinished(piter)) {
                const auto pentry = AiParamIteratorGetNext(piter);
                auto pname = AiParamGetName(pentry);
                if (strcmp(pname, "name") == 0) {
                    continue;
                }
                const auto ptype = static_cast<uint8_t>(AiParamGetType(pentry));
                export_parameter(arnold_node, shader, pname, ptype, false);
            }
            AiParamIteratorDestroy(piter);
            auto puiter = AiNodeGetUserParamIterator(arnold_node);
            while (!AiUserParamIteratorFinished(puiter)) {
                const auto pentry = AiUserParamIteratorGetNext(puiter);
                auto pname = AiUserParamGetName(pentry);
                const auto ptype = static_cast<uint8_t>(AiUserParamGetType(pentry));
                export_parameter(arnold_node, shader, pname, ptype, true);
            }
            AiUserParamIteratorDestroy(puiter);
            return shader_path;
        }
    }
}

void AiShaderExport::bind_material(const SdfPath& material_path, const SdfPath& shape_path) {
    auto shape_prim = m_stage->GetPrimAtPath(shape_path);
    if (!shape_prim.IsValid()) {
        return;
    }
    auto material_prim = m_stage->GetPrimAtPath(material_path);
    if (!material_prim.IsValid()) {
        return;
    }
    if (shape_prim.HasRelationship(UsdShadeTokens->materialBinding)) {
        auto rel = shape_prim.GetRelationship(UsdShadeTokens->materialBinding);
        rel.ClearTargets(true);
        rel.AppendTarget(material_path);
    } else {
        auto rel = shape_prim.CreateRelationship(UsdShadeTokens->materialBinding);
        rel.AppendTarget(material_path);
    }
}

SdfPath
AiShaderExport::export_material(const char* material_name, AtNode* surf_shader, AtNode* disp_shader) {
    auto material_path = m_shaders_scope.AppendChild(TfToken(material_name));
    auto material_prim = m_stage->GetPrimAtPath(material_path);
    if (material_prim.IsValid()) {
        // already exists and setup
        return material_path;
    }
    auto material = UsdAiMaterialAPI(UsdShadeMaterial::Define(m_stage, material_path));

    if (surf_shader != nullptr) {
        auto surf_path = export_arnold_node(surf_shader, material_path);
        if (!surf_path.IsEmpty()) {
            auto rel = material.CreateSurfaceRel();
            rel.AppendTarget(surf_path);
        }
    }

    if (disp_shader != nullptr) {
        // FIXME: it's unclear why disp uses m_shaders_scope and surf uses material_path...
        auto disp_path = export_arnold_node(disp_shader, m_shaders_scope);
        if (!disp_path.IsEmpty()) {
            auto rel = material.CreateDisplacementRel();
            rel.AppendTarget(disp_path);
        }
    }
    return material_path;
}

void _collapse_shaders(UsdPrim prim) {
    if (!prim.IsA<UsdGeomXform>()) {
        return;
    }

    auto children = prim.GetAllChildren();
    if (children.empty()) {
        return;
    }
    SdfPathVector materials;
    bool first = true;
    for (auto child : children) {
        _collapse_shaders(child);
        if (child.HasRelationship(UsdShadeTokens->materialBinding)) {
            if (first) {
                child.GetRelationship(UsdShadeTokens->materialBinding).GetTargets(&materials);
                first = false;
            } else {
                SdfPathVector test_materials;
                child.GetRelationship(UsdShadeTokens->materialBinding).GetTargets(&test_materials);
                if (materials != test_materials) {
                    materials.clear();
                    break;
                }
            }
        } else {
            materials.clear();
            break;
        }
    }

    if (!materials.empty()) {
        for (auto child : children) {
            if (child.HasRelationship(UsdShadeTokens->materialBinding)) {
                child.RemoveProperty(UsdShadeTokens->materialBinding);
            }
        }
        prim.CreateRelationship(UsdShadeTokens->materialBinding).SetTargets(materials);
    }
}

void AiShaderExport::collapse_shaders() { 
    tbb::tick_count tc = tbb::tick_count::now();
    auto prim_range = m_stage->Traverse();
    for (auto stage_iter = prim_range.begin(); stage_iter != prim_range.end(); ++stage_iter) {
        if (stage_iter.IsPostVisit()) {
            continue;
        }

        // shaders and instances are in a scope, everything else is simple hierarchy
        // with mostly shader assignments
        if (!stage_iter->IsA<UsdGeomScope>()) {
            _collapse_shaders(*stage_iter);
        }
        stage_iter.PruneChildren();
    }
    std::cerr << "[usdAi] Collapsing shader assignments took: "
              << (tbb::tick_count::now() - tc).seconds() << std::endl;
}


PXR_NAMESPACE_CLOSE_SCOPE
