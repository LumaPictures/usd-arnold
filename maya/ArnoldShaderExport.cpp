#include "ArnoldShaderExport.h"

#include "pxr/base/tf/getenv.h"
#include "pxr/usd/usdShade/material.h"
#include "pxr/usd/usdAi/aiMaterialAPI.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

#include <scene/MayaScene.h>
#include <translators/NodeTranslator.h>

PXR_NAMESPACE_OPEN_SCOPE

namespace {
    AtNode* mtoa_export_node(const MObject& obj, const char* plugName) {
        MStatus status;
        MFnDependencyNode node(obj, &status);
        if (!status) { return 0; }
        auto* arnoldSession = CMayaScene::GetArnoldSession();
        if (arnoldSession == nullptr) { return nullptr; }
        auto* trans = arnoldSession->ExportNode(node.findPlug(plugName));
#if MTOA12
        return trans == nullptr ? nullptr : trans->GetArnoldRootNode();
#else
        return trans == nullptr ? nullptr : trans->GetArnoldNode();
#endif
    }
}

ArnoldShaderExport::ArnoldShaderExport(const UsdStageRefPtr& _stage,
                                       const UsdTimeCode& _time_code,
                                       const SdfPath& parent_scope,
                                       const PxrUsdMayaUtil::MDagPathMap<SdfPath>::Type& dag_to_usd) :
    AiShaderExport(_stage, parent_scope, _time_code), m_dag_to_usd(dag_to_usd) {
    const auto transform_assignment = TfGetenv("PXR_MAYA_TRANSFORM_ASSIGNMENT", "disable");
    if (transform_assignment == "common") {
        m_transform_assignment = TRANSFORM_ASSIGNMENT_COMMON;
    } else if (transform_assignment == "full") {
        m_transform_assignment = TRANSFORM_ASSIGNMENT_FULL;
    } else {
        m_transform_assignment = TRANSFORM_ASSIGNMENT_DISABLE;
    }
    CMayaScene::End();
    AiMsgSetConsoleFlags(AI_LOG_NONE);
    CMayaScene::Begin(MTOA_SESSION_ASS);
    AiMsgSetConsoleFlags(AI_LOG_NONE);
}

ArnoldShaderExport::~ArnoldShaderExport() {
    CMayaScene::End();
}

SdfPath
ArnoldShaderExport::export_shading_engine(MObject obj) {
    if (!obj.hasFn(MFn::kShadingEngine)) {
        return SdfPath();
    }

    auto surf_shader = mtoa_export_node(obj, "message");

    // we can't store the material in the map
    MFnDependencyNode node(obj);

    AtNode* disp_shader = nullptr;
    auto disp_plug = node.findPlug("displacementShader");
    MPlugArray conns;
    disp_plug.connectedTo(conns, true, false);
    if (conns.length() > 0) {
        auto disp_obj = conns[0].node();
        disp_shader = mtoa_export_node(disp_obj, conns[0].partialName(false, false, false, false, false, true).asChar());
    }
    return export_material(node.name().asChar(), surf_shader, disp_shader);
}

void
ArnoldShaderExport::setup_shader(const MDagPath& dg, const SdfPath& path) {
    auto obj = dg.node();
    if (obj.hasFn(MFn::kTransform) || obj.hasFn(MFn::kLocator)) { return; }

    if (obj.hasFn(MFn::kPluginShape)) {
        MFnDependencyNode dn(obj);
        if (dn.typeName() == "vdb_visualizer") {
            auto* volume_node = mtoa_export_node(obj, "message");
            static const AtString volumeString("volume");
            if (!AiNodeIs(volume_node, volumeString)) {
                return;
            }
            const auto* linked_shader = reinterpret_cast<const AtNode*>(AiNodeGetPtr(volume_node, "shader"));
            if (linked_shader == nullptr) {
                return;
            }
            std::string cleaned_volume_name = AiNodeGetName(linked_shader);
            clean_arnold_name(cleaned_volume_name);

            // start: below here is roughly the same as AiShaderExport::export_material
            auto material_path = m_shaders_scope.AppendChild(TfToken(cleaned_volume_name));

            UsdAiMaterialAPI material;
            auto material_prim = m_stage->GetPrimAtPath(material_path);
            if (!material_prim.IsValid()) {
                material = UsdAiMaterialAPI(UsdShadeMaterial::Define(m_stage, material_path));
                material_prim = material.GetPrim();
            }
            else {
                // FIXME: why would a material already exist for a vdb_visualizer?
                material = UsdAiMaterialAPI(material_prim);
            }

            auto linked_path = export_arnold_node(linked_shader, material_path);
            if (!linked_path.IsEmpty()) {
                auto rel = material.GetSurfaceRel();
                if (rel) {
                    rel.ClearTargets(true);
                    rel.AddTarget(linked_path);
                } else {
                    rel = material.CreateSurfaceRel();
                    rel.AddTarget(linked_path);
                }
            }
            // end
            bind_material(material_path, path);
            return;
        }
    }

    auto get_shading_engine_obj = [] (MObject shape_obj, unsigned int instance_num) -> MObject {
        constexpr auto out_shader_plug = "instObjGroups";

        MFnDependencyNode node (shape_obj);
        auto plug = node.findPlug(out_shader_plug);
        MPlugArray conns;
        plug.elementByLogicalIndex(instance_num).connectedTo(conns, false, true);
        const auto conns_length = conns.length();
        for (auto i = decltype(conns_length){0}; i < conns_length; ++i) {
            const auto splug = conns[i];
            const auto sobj = splug.node();
            if (sobj.apiType() == MFn::kShadingEngine) {
                return sobj;
            }
        }
        return MObject();
    };

    auto is_initial_group = [] (MObject tobj) -> bool {
        constexpr auto initial_shading_group_name = "initialShadingGroup";
        return MFnDependencyNode(tobj).name() == initial_shading_group_name;
    };

    auto material_assignment = get_shading_engine_obj(obj, dg.instanceNumber());
    // we are checking for transform assignments as well
    if (m_transform_assignment == TRANSFORM_ASSIGNMENT_FULL &&
        (material_assignment.isNull() || is_initial_group(material_assignment))) {
        auto dag_it = dg;
        dag_it.pop();
        for (; dag_it.length() > 0; dag_it.pop()) {
            const auto it = m_dag_to_usd.find(dag_it);
            if (it != m_dag_to_usd.end()) {
                const auto transform_assignment = get_shading_engine_obj(dag_it.node(), dag_it.instanceNumber());
                if (!transform_assignment.isNull() && !is_initial_group(transform_assignment)) {
                    const auto shader_path = export_shading_engine(transform_assignment);
                    if (shader_path.IsEmpty()) {
                        return;
                    }
                    bind_material(shader_path, it->second.GetPrimPath());
                    return;
                }
            }
        }
    }

    if (material_assignment.isNull()) {
        material_assignment = get_shading_engine_obj(obj, 0);
    }

    const auto shader_path = export_shading_engine(material_assignment);
    if (shader_path.IsEmpty()) {
        return;
    }
    bind_material(shader_path, path);
}


void ArnoldShaderExport::setup_shaders() {
    for (const auto& it : m_dag_to_usd) {
        setup_shader(it.first, it.second);
    }
    if (m_transform_assignment == TRANSFORM_ASSIGNMENT_COMMON) {
        collapse_shaders();
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
