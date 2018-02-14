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
// GENERATED FILE.  DO NOT EDIT.
#include <boost/python/class.hpp>
#include "pxr/usd/usdAi/tokens.h"

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

// Helper to return a static token as a string.  We wrap tokens as Python
// strings and for some reason simply wrapping the token using def_readonly
// bypasses to-Python conversion, leading to the error that there's no
// Python type for the C++ TfToken type.  So we wrap this functor instead.
class _WrapStaticToken {
public:
    _WrapStaticToken(const TfToken* token) : _token(token) { }

    std::string operator()() const
    {
        return _token->GetString();
    }

private:
    const TfToken* _token;
};

template <typename T>
void
_AddToken(T& cls, const char* name, const TfToken& token)
{
    cls.add_static_property(name,
                            boost::python::make_function(
                                _WrapStaticToken(&token),
                                boost::python::return_value_policy<
                                    boost::python::return_by_value>(),
                                boost::mpl::vector1<std::string>()));
}

} // anonymous

void wrapUsdAiTokens()
{
    boost::python::class_<UsdAiTokensType, boost::noncopyable>
        cls("Tokens", boost::python::no_init);
    _AddToken(cls, "aiAov", UsdAiTokens->aiAov);
    _AddToken(cls, "aiAutobump_visibilityCamera", UsdAiTokens->aiAutobump_visibilityCamera);
    _AddToken(cls, "aiAutobump_visibilityDiffuse_reflect", UsdAiTokens->aiAutobump_visibilityDiffuse_reflect);
    _AddToken(cls, "aiAutobump_visibilityDiffuse_transmit", UsdAiTokens->aiAutobump_visibilityDiffuse_transmit);
    _AddToken(cls, "aiAutobump_visibilityShadow", UsdAiTokens->aiAutobump_visibilityShadow);
    _AddToken(cls, "aiAutobump_visibilitySpecular_reflect", UsdAiTokens->aiAutobump_visibilitySpecular_reflect);
    _AddToken(cls, "aiAutobump_visibilitySpecular_transmit", UsdAiTokens->aiAutobump_visibilitySpecular_transmit);
    _AddToken(cls, "aiAutobump_visibilitySubsurface", UsdAiTokens->aiAutobump_visibilitySubsurface);
    _AddToken(cls, "aiAutobump_visibilityVolume", UsdAiTokens->aiAutobump_visibilityVolume);
    _AddToken(cls, "aiDisp_autobump", UsdAiTokens->aiDisp_autobump);
    _AddToken(cls, "aiDisp_height", UsdAiTokens->aiDisp_height);
    _AddToken(cls, "aiDisp_padding", UsdAiTokens->aiDisp_padding);
    _AddToken(cls, "aiDisp_zero_value", UsdAiTokens->aiDisp_zero_value);
    _AddToken(cls, "aiDisplacement", UsdAiTokens->aiDisplacement);
    _AddToken(cls, "aiLight_group", UsdAiTokens->aiLight_group);
    _AddToken(cls, "aiMatte", UsdAiTokens->aiMatte);
    _AddToken(cls, "aiOpaque", UsdAiTokens->aiOpaque);
    _AddToken(cls, "aiRay_bias", UsdAiTokens->aiRay_bias);
    _AddToken(cls, "aiReceive_shadows", UsdAiTokens->aiReceive_shadows);
    _AddToken(cls, "aiSelf_shadows", UsdAiTokens->aiSelf_shadows);
    _AddToken(cls, "aiShadow_group", UsdAiTokens->aiShadow_group);
    _AddToken(cls, "aiSidednessCamera", UsdAiTokens->aiSidednessCamera);
    _AddToken(cls, "aiSidednessDiffuse_reflect", UsdAiTokens->aiSidednessDiffuse_reflect);
    _AddToken(cls, "aiSidednessDiffuse_transmit", UsdAiTokens->aiSidednessDiffuse_transmit);
    _AddToken(cls, "aiSidednessShadow", UsdAiTokens->aiSidednessShadow);
    _AddToken(cls, "aiSidednessSpecular_reflect", UsdAiTokens->aiSidednessSpecular_reflect);
    _AddToken(cls, "aiSidednessSpecular_transmit", UsdAiTokens->aiSidednessSpecular_transmit);
    _AddToken(cls, "aiSidednessSubsurface", UsdAiTokens->aiSidednessSubsurface);
    _AddToken(cls, "aiSidednessVolume", UsdAiTokens->aiSidednessVolume);
    _AddToken(cls, "aiSmoothing", UsdAiTokens->aiSmoothing);
    _AddToken(cls, "aiSubdiv_adaptive_error", UsdAiTokens->aiSubdiv_adaptive_error);
    _AddToken(cls, "aiSubdiv_adaptive_metric", UsdAiTokens->aiSubdiv_adaptive_metric);
    _AddToken(cls, "aiSubdiv_adaptive_space", UsdAiTokens->aiSubdiv_adaptive_space);
    _AddToken(cls, "aiSubdiv_dicing_camera", UsdAiTokens->aiSubdiv_dicing_camera);
    _AddToken(cls, "aiSubdiv_iterations", UsdAiTokens->aiSubdiv_iterations);
    _AddToken(cls, "aiSubdiv_smooth_derivs", UsdAiTokens->aiSubdiv_smooth_derivs);
    _AddToken(cls, "aiSubdiv_type", UsdAiTokens->aiSubdiv_type);
    _AddToken(cls, "aiSubdiv_uv_smoothing", UsdAiTokens->aiSubdiv_uv_smoothing);
    _AddToken(cls, "aiSurface", UsdAiTokens->aiSurface);
    _AddToken(cls, "aiTransform_type", UsdAiTokens->aiTransform_type);
    _AddToken(cls, "aiUse_light_group", UsdAiTokens->aiUse_light_group);
    _AddToken(cls, "aiUse_shadow_group", UsdAiTokens->aiUse_shadow_group);
    _AddToken(cls, "aiVisibilityCamera", UsdAiTokens->aiVisibilityCamera);
    _AddToken(cls, "aiVisibilityDiffuse_reflect", UsdAiTokens->aiVisibilityDiffuse_reflect);
    _AddToken(cls, "aiVisibilityDiffuse_transmit", UsdAiTokens->aiVisibilityDiffuse_transmit);
    _AddToken(cls, "aiVisibilityShadow", UsdAiTokens->aiVisibilityShadow);
    _AddToken(cls, "aiVisibilitySpecular_reflect", UsdAiTokens->aiVisibilitySpecular_reflect);
    _AddToken(cls, "aiVisibilitySpecular_transmit", UsdAiTokens->aiVisibilitySpecular_transmit);
    _AddToken(cls, "aiVisibilitySubsurface", UsdAiTokens->aiVisibilitySubsurface);
    _AddToken(cls, "aiVisibilityVolume", UsdAiTokens->aiVisibilityVolume);
    _AddToken(cls, "aRRAY", UsdAiTokens->aRRAY);
    _AddToken(cls, "auto_", UsdAiTokens->auto_);
    _AddToken(cls, "bOOL", UsdAiTokens->bOOL);
    _AddToken(cls, "bYTE", UsdAiTokens->bYTE);
    _AddToken(cls, "catclark", UsdAiTokens->catclark);
    _AddToken(cls, "data", UsdAiTokens->data);
    _AddToken(cls, "dataType", UsdAiTokens->dataType);
    _AddToken(cls, "driver", UsdAiTokens->driver);
    _AddToken(cls, "edge_length", UsdAiTokens->edge_length);
    _AddToken(cls, "filename", UsdAiTokens->filename);
    _AddToken(cls, "filter", UsdAiTokens->filter);
    _AddToken(cls, "flatness", UsdAiTokens->flatness);
    _AddToken(cls, "fLOAT", UsdAiTokens->fLOAT);
    _AddToken(cls, "infoId", UsdAiTokens->infoId);
    _AddToken(cls, "iNT", UsdAiTokens->iNT);
    _AddToken(cls, "linear", UsdAiTokens->linear);
    _AddToken(cls, "lPE", UsdAiTokens->lPE);
    _AddToken(cls, "mATRIX", UsdAiTokens->mATRIX);
    _AddToken(cls, "name", UsdAiTokens->name);
    _AddToken(cls, "nODE", UsdAiTokens->nODE);
    _AddToken(cls, "none", UsdAiTokens->none);
    _AddToken(cls, "object", UsdAiTokens->object);
    _AddToken(cls, "path", UsdAiTokens->path);
    _AddToken(cls, "pin_borders", UsdAiTokens->pin_borders);
    _AddToken(cls, "pin_corners", UsdAiTokens->pin_corners);
    _AddToken(cls, "pOINTER", UsdAiTokens->pOINTER);
    _AddToken(cls, "raster", UsdAiTokens->raster);
    _AddToken(cls, "rGB", UsdAiTokens->rGB);
    _AddToken(cls, "rGBA", UsdAiTokens->rGBA);
    _AddToken(cls, "rotate_about_center", UsdAiTokens->rotate_about_center);
    _AddToken(cls, "rotate_about_origin", UsdAiTokens->rotate_about_origin);
    _AddToken(cls, "size", UsdAiTokens->size);
    _AddToken(cls, "smooth", UsdAiTokens->smooth);
    _AddToken(cls, "step_size", UsdAiTokens->step_size);
    _AddToken(cls, "uINT", UsdAiTokens->uINT);
    _AddToken(cls, "userPrefix", UsdAiTokens->userPrefix);
    _AddToken(cls, "vECTOR", UsdAiTokens->vECTOR);
    _AddToken(cls, "vECTOR2", UsdAiTokens->vECTOR2);
}
