//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxr/usd/usdAi/aiShaderExport.h"

#include "pxr/usd/sdf/primSpec.h"

#include "pxr/usd/usd/pyConversions.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include <boost/python/class.hpp>
#include <boost/python/import.hpp>

#include <string>

using std::string;

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

static SdfPath
_export_material(AiShaderExport &self, const char* material_name,
                 const object& surf_shader, const object& disp_shader)
{
    static object ctypes_addressof = import("ctypes").attr("addressof");
    AtNode* surf_node = nullptr;
    if (!surf_shader.is_none()) {
        surf_node = *reinterpret_cast<AtNode**>(uintptr_t(extract<uintptr_t>(ctypes_addressof(surf_shader))));
    }
    AtNode* disp_node = nullptr;
    if (!disp_shader.is_none()) {
        disp_node = *reinterpret_cast<AtNode**>(uintptr_t(extract<uintptr_t>(ctypes_addressof(disp_shader))));
    }
    return self.export_material(material_name, surf_node, disp_node);
}

} // anonymous namespace 

void wrapAiShaderExport()
{
    typedef AiShaderExport This;

    class_<This>
        cls("AiShaderExport", no_init);

    cls
        .def(init<const UsdStagePtr &, const UsdTimeCode &, const std::string &>(
             (arg("_stage"),
              arg("_time_code") = UsdTimeCode::Default(),
              arg("parent_scope") = string())))
        // .def("__init__", __init__,
        //      (arg("_stage"),
        //       arg("_time_code") = UsdTimeCode::Default(),
        //       arg("parent_scope") = string()))
        .def("bind_material", &This::bind_material,
             (arg("shader_path"), arg("shape_path"))) 
        .def("export_material", &_export_material,
             (arg("material_name"), arg("surf_shader"), arg("disp_shader")))
        ;
}
