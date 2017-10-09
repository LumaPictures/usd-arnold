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
#include "pxr/usd/usdAi/aiShapeAPI.h"
#include "pxr/usd/usd/schemaBase.h"

#include "pxr/usd/sdf/primSpec.h"

#include "pxr/usd/usd/pyConversions.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include <boost/python.hpp>

#include <string>

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

#define WRAP_CUSTOM                                                     \
    template <class Cls> static void _CustomWrapCode(Cls &_class)

// fwd decl.
WRAP_CUSTOM;

        
static UsdAttribute
_CreateAiOpaqueAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiOpaqueAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiMatteAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiMatteAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiReceiveShadowsAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiReceiveShadowsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiRayBiasAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiRayBiasAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateAiSelfShadowsAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSelfShadowsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiTransformTypeAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiTransformTypeAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToCameraAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToCameraAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToShadowAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToShadowAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToDiffuseTransmitAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToDiffuseTransmitAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToSpecularTransmitAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToSpecularTransmitAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToVolumeAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToVolumeAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToDiffuseReflectAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToDiffuseReflectAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToSpecularReflectAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToSpecularReflectAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiVisibleToSubsurfaceAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiVisibleToSubsurfaceAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToCameraAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToCameraAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToShadowAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToShadowAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToDiffuseTransmitAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToDiffuseTransmitAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToSpecularTransmitAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToSpecularTransmitAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToVolumeAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToVolumeAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToDiffuseReflectAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToDiffuseReflectAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToSpecularReflectAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToSpecularReflectAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDoubleSidedToSubsurfaceAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDoubleSidedToSubsurfaceAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToCameraAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToCameraAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToShadowAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToShadowAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToDiffuseTransmitAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToDiffuseTransmitAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToSpecularTransmitAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToSpecularTransmitAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToVolumeAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToVolumeAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToDiffuseReflectAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToDiffuseReflectAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToSpecularReflectAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToSpecularReflectAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiAutobumpVisibleToSubsurfaceAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiAutobumpVisibleToSubsurfaceAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiUseLightGroupAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiUseLightGroupAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiUseShadowGroupAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiUseShadowGroupAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiSmoothingAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSmoothingAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiSubdivTypeAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSubdivTypeAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}
        
static UsdAttribute
_CreateAiSubdivIterationsAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSubdivIterationsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->UInt), writeSparsely);
}
        
static UsdAttribute
_CreateAiSubdivAdaptiveErrorAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSubdivAdaptiveErrorAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateAiSubdivAdaptiveMetricAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSubdivAdaptiveMetricAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}
        
static UsdAttribute
_CreateAiSubdivAdaptiveSpaceAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSubdivAdaptiveSpaceAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}
        
static UsdAttribute
_CreateAiSubdivUVSmoothingAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSubdivUVSmoothingAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}
        
static UsdAttribute
_CreateAiSubdivSmoothDerivsAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiSubdivSmoothDerivsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateAiDispPaddingAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDispPaddingAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateAiDispHeightAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDispHeightAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateAiDispZeroValueAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDispZeroValueAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateAiDispAutobumpAttr(UsdAiShapeAPI &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateAiDispAutobumpAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}

} // anonymous namespace

void wrapUsdAiShapeAPI()
{
    typedef UsdAiShapeAPI This;

    class_<This, bases<UsdSchemaBase> >
        cls("AiShapeAPI");

    cls
        .def(init<UsdPrim>(arg("prim")))
        .def(init<UsdSchemaBase const&>(arg("schemaObj")))
        .def(TfTypePythonClass())

        .def("Get", &This::Get, (arg("stage"), arg("path")))
        .staticmethod("Get")


        .def("GetSchemaAttributeNames",
             &This::GetSchemaAttributeNames,
             arg("includeInherited")=true,
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetSchemaAttributeNames")

        .def("_GetStaticTfType", (TfType const &(*)()) TfType::Find<This>,
             return_value_policy<return_by_value>())
        .staticmethod("_GetStaticTfType")

        .def(!self)

        
        .def("GetAiOpaqueAttr",
             &This::GetAiOpaqueAttr)
        .def("CreateAiOpaqueAttr",
             &_CreateAiOpaqueAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiMatteAttr",
             &This::GetAiMatteAttr)
        .def("CreateAiMatteAttr",
             &_CreateAiMatteAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiReceiveShadowsAttr",
             &This::GetAiReceiveShadowsAttr)
        .def("CreateAiReceiveShadowsAttr",
             &_CreateAiReceiveShadowsAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiRayBiasAttr",
             &This::GetAiRayBiasAttr)
        .def("CreateAiRayBiasAttr",
             &_CreateAiRayBiasAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSelfShadowsAttr",
             &This::GetAiSelfShadowsAttr)
        .def("CreateAiSelfShadowsAttr",
             &_CreateAiSelfShadowsAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiTransformTypeAttr",
             &This::GetAiTransformTypeAttr)
        .def("CreateAiTransformTypeAttr",
             &_CreateAiTransformTypeAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToCameraAttr",
             &This::GetAiVisibleToCameraAttr)
        .def("CreateAiVisibleToCameraAttr",
             &_CreateAiVisibleToCameraAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToShadowAttr",
             &This::GetAiVisibleToShadowAttr)
        .def("CreateAiVisibleToShadowAttr",
             &_CreateAiVisibleToShadowAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToDiffuseTransmitAttr",
             &This::GetAiVisibleToDiffuseTransmitAttr)
        .def("CreateAiVisibleToDiffuseTransmitAttr",
             &_CreateAiVisibleToDiffuseTransmitAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToSpecularTransmitAttr",
             &This::GetAiVisibleToSpecularTransmitAttr)
        .def("CreateAiVisibleToSpecularTransmitAttr",
             &_CreateAiVisibleToSpecularTransmitAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToVolumeAttr",
             &This::GetAiVisibleToVolumeAttr)
        .def("CreateAiVisibleToVolumeAttr",
             &_CreateAiVisibleToVolumeAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToDiffuseReflectAttr",
             &This::GetAiVisibleToDiffuseReflectAttr)
        .def("CreateAiVisibleToDiffuseReflectAttr",
             &_CreateAiVisibleToDiffuseReflectAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToSpecularReflectAttr",
             &This::GetAiVisibleToSpecularReflectAttr)
        .def("CreateAiVisibleToSpecularReflectAttr",
             &_CreateAiVisibleToSpecularReflectAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiVisibleToSubsurfaceAttr",
             &This::GetAiVisibleToSubsurfaceAttr)
        .def("CreateAiVisibleToSubsurfaceAttr",
             &_CreateAiVisibleToSubsurfaceAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToCameraAttr",
             &This::GetAiDoubleSidedToCameraAttr)
        .def("CreateAiDoubleSidedToCameraAttr",
             &_CreateAiDoubleSidedToCameraAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToShadowAttr",
             &This::GetAiDoubleSidedToShadowAttr)
        .def("CreateAiDoubleSidedToShadowAttr",
             &_CreateAiDoubleSidedToShadowAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToDiffuseTransmitAttr",
             &This::GetAiDoubleSidedToDiffuseTransmitAttr)
        .def("CreateAiDoubleSidedToDiffuseTransmitAttr",
             &_CreateAiDoubleSidedToDiffuseTransmitAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToSpecularTransmitAttr",
             &This::GetAiDoubleSidedToSpecularTransmitAttr)
        .def("CreateAiDoubleSidedToSpecularTransmitAttr",
             &_CreateAiDoubleSidedToSpecularTransmitAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToVolumeAttr",
             &This::GetAiDoubleSidedToVolumeAttr)
        .def("CreateAiDoubleSidedToVolumeAttr",
             &_CreateAiDoubleSidedToVolumeAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToDiffuseReflectAttr",
             &This::GetAiDoubleSidedToDiffuseReflectAttr)
        .def("CreateAiDoubleSidedToDiffuseReflectAttr",
             &_CreateAiDoubleSidedToDiffuseReflectAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToSpecularReflectAttr",
             &This::GetAiDoubleSidedToSpecularReflectAttr)
        .def("CreateAiDoubleSidedToSpecularReflectAttr",
             &_CreateAiDoubleSidedToSpecularReflectAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDoubleSidedToSubsurfaceAttr",
             &This::GetAiDoubleSidedToSubsurfaceAttr)
        .def("CreateAiDoubleSidedToSubsurfaceAttr",
             &_CreateAiDoubleSidedToSubsurfaceAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToCameraAttr",
             &This::GetAiAutobumpVisibleToCameraAttr)
        .def("CreateAiAutobumpVisibleToCameraAttr",
             &_CreateAiAutobumpVisibleToCameraAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToShadowAttr",
             &This::GetAiAutobumpVisibleToShadowAttr)
        .def("CreateAiAutobumpVisibleToShadowAttr",
             &_CreateAiAutobumpVisibleToShadowAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToDiffuseTransmitAttr",
             &This::GetAiAutobumpVisibleToDiffuseTransmitAttr)
        .def("CreateAiAutobumpVisibleToDiffuseTransmitAttr",
             &_CreateAiAutobumpVisibleToDiffuseTransmitAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToSpecularTransmitAttr",
             &This::GetAiAutobumpVisibleToSpecularTransmitAttr)
        .def("CreateAiAutobumpVisibleToSpecularTransmitAttr",
             &_CreateAiAutobumpVisibleToSpecularTransmitAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToVolumeAttr",
             &This::GetAiAutobumpVisibleToVolumeAttr)
        .def("CreateAiAutobumpVisibleToVolumeAttr",
             &_CreateAiAutobumpVisibleToVolumeAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToDiffuseReflectAttr",
             &This::GetAiAutobumpVisibleToDiffuseReflectAttr)
        .def("CreateAiAutobumpVisibleToDiffuseReflectAttr",
             &_CreateAiAutobumpVisibleToDiffuseReflectAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToSpecularReflectAttr",
             &This::GetAiAutobumpVisibleToSpecularReflectAttr)
        .def("CreateAiAutobumpVisibleToSpecularReflectAttr",
             &_CreateAiAutobumpVisibleToSpecularReflectAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiAutobumpVisibleToSubsurfaceAttr",
             &This::GetAiAutobumpVisibleToSubsurfaceAttr)
        .def("CreateAiAutobumpVisibleToSubsurfaceAttr",
             &_CreateAiAutobumpVisibleToSubsurfaceAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiUseLightGroupAttr",
             &This::GetAiUseLightGroupAttr)
        .def("CreateAiUseLightGroupAttr",
             &_CreateAiUseLightGroupAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiUseShadowGroupAttr",
             &This::GetAiUseShadowGroupAttr)
        .def("CreateAiUseShadowGroupAttr",
             &_CreateAiUseShadowGroupAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSmoothingAttr",
             &This::GetAiSmoothingAttr)
        .def("CreateAiSmoothingAttr",
             &_CreateAiSmoothingAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSubdivTypeAttr",
             &This::GetAiSubdivTypeAttr)
        .def("CreateAiSubdivTypeAttr",
             &_CreateAiSubdivTypeAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSubdivIterationsAttr",
             &This::GetAiSubdivIterationsAttr)
        .def("CreateAiSubdivIterationsAttr",
             &_CreateAiSubdivIterationsAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSubdivAdaptiveErrorAttr",
             &This::GetAiSubdivAdaptiveErrorAttr)
        .def("CreateAiSubdivAdaptiveErrorAttr",
             &_CreateAiSubdivAdaptiveErrorAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSubdivAdaptiveMetricAttr",
             &This::GetAiSubdivAdaptiveMetricAttr)
        .def("CreateAiSubdivAdaptiveMetricAttr",
             &_CreateAiSubdivAdaptiveMetricAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSubdivAdaptiveSpaceAttr",
             &This::GetAiSubdivAdaptiveSpaceAttr)
        .def("CreateAiSubdivAdaptiveSpaceAttr",
             &_CreateAiSubdivAdaptiveSpaceAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSubdivUVSmoothingAttr",
             &This::GetAiSubdivUVSmoothingAttr)
        .def("CreateAiSubdivUVSmoothingAttr",
             &_CreateAiSubdivUVSmoothingAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiSubdivSmoothDerivsAttr",
             &This::GetAiSubdivSmoothDerivsAttr)
        .def("CreateAiSubdivSmoothDerivsAttr",
             &_CreateAiSubdivSmoothDerivsAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDispPaddingAttr",
             &This::GetAiDispPaddingAttr)
        .def("CreateAiDispPaddingAttr",
             &_CreateAiDispPaddingAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDispHeightAttr",
             &This::GetAiDispHeightAttr)
        .def("CreateAiDispHeightAttr",
             &_CreateAiDispHeightAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDispZeroValueAttr",
             &This::GetAiDispZeroValueAttr)
        .def("CreateAiDispZeroValueAttr",
             &_CreateAiDispZeroValueAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetAiDispAutobumpAttr",
             &This::GetAiDispAutobumpAttr)
        .def("CreateAiDispAutobumpAttr",
             &_CreateAiDispAutobumpAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))

        
        .def("GetAiLightGroupRel",
             &This::GetAiLightGroupRel)
        .def("CreateAiLightGroupRel",
             &This::CreateAiLightGroupRel)
        
        .def("GetAiShadowGroupRel",
             &This::GetAiShadowGroupRel)
        .def("CreateAiShadowGroupRel",
             &This::CreateAiShadowGroupRel)
        
        .def("GetAiSubdiv_dicing_cameraRel",
             &This::GetAiSubdiv_dicing_cameraRel)
        .def("CreateAiSubdiv_dicing_cameraRel",
             &This::CreateAiSubdiv_dicing_cameraRel)
    ;

    _CustomWrapCode(cls);
}

// ===================================================================== //
// Feel free to add custom code below this line, it will be preserved by 
// the code generator.  The entry point for your custom code should look
// minimally like the following:
//
// WRAP_CUSTOM {
//     _class
//         .def("MyCustomMethod", ...)
//     ;
// }
//
// Of course any other ancillary or support code may be provided.
// 
// Just remember to wrap code in the appropriate delimiters:
// 'namespace {', '}'.
//
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--

namespace {

WRAP_CUSTOM {
    _class
        .def("ComputeVisibility",
             &UsdAiShapeAPI::ComputeVisibility)
        .def("ComputeSidedness",
             &UsdAiShapeAPI::ComputeSidedness)
        .def("ComputeAutobumpVisibility",
             &UsdAiShapeAPI::ComputeAutobumpVisibility)
        ;
}

}
