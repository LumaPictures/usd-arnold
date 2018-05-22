#include "renderDelegate.h"

#include <pxr/imaging/glf/glew.h>
#include <pxr/imaging/hd/resourceRegistry.h>
#include <pxr/imaging/hd/camera.h>
#include <pxr/imaging/hd/bprim.h>
#include <pxr/imaging/hd/tokens.h>
#include <pxr/imaging/hd/instancer.h>
#include <pxr/imaging/hd/rprim.h>

PXR_NAMESPACE_OPEN_SCOPE

const TfTokenVector HdAiRenderDelegate::SUPPORTED_RPRIM_TYPES = {
    HdPrimTypeTokens->mesh,
};

const TfTokenVector HdAiRenderDelegate::SUPPORTED_SPRIM_TYPES = {
    HdPrimTypeTokens->camera,
};

const TfTokenVector HdAiRenderDelegate::SUPPORTED_BPRIM_TYPES = {
};

HdAiRenderDelegate::HdAiRenderDelegate() {
}

HdAiRenderDelegate::~HdAiRenderDelegate() {
}

HdRenderParam*
HdAiRenderDelegate::GetRenderParam() const {
    return nullptr;
}

void
HdAiRenderDelegate::CommitResources(HdChangeTracker *tracker) {
}

TfTokenVector const&
HdAiRenderDelegate::GetSupportedRprimTypes() const {
    return SUPPORTED_RPRIM_TYPES;
}

TfTokenVector const&
HdAiRenderDelegate::GetSupportedSprimTypes() const {
    return SUPPORTED_SPRIM_TYPES;
}

TfTokenVector const&
HdAiRenderDelegate::GetSupportedBprimTypes() const {
    return SUPPORTED_BPRIM_TYPES;
}

HdResourceRegistrySharedPtr
HdAiRenderDelegate::GetResourceRegistry() const {
    return nullptr;
}

HdRenderPassSharedPtr
HdAiRenderDelegate::CreateRenderPass(HdRenderIndex *index,
                                         HdRprimCollection const& collection) {
    return nullptr;
}

HdInstancer *
HdAiRenderDelegate::CreateInstancer(HdSceneDelegate *delegate,
                                        SdfPath const& id,
                                        SdfPath const& instancerId) {
    return nullptr;
}

void
HdAiRenderDelegate::DestroyInstancer(HdInstancer *instancer) {
    delete instancer;
}

HdRprim *
HdAiRenderDelegate::CreateRprim(TfToken const& typeId,
                                    SdfPath const& rprimId,
                                    SdfPath const& instancerId) {
    if (typeId == HdPrimTypeTokens->mesh) {
        return nullptr;
    } else {
        TF_CODING_ERROR("Unknown Rprim Type %s", typeId.GetText());
    }

    return nullptr;
}

void
HdAiRenderDelegate::DestroyRprim(HdRprim *rPrim) {
    delete rPrim;
}

HdSprim *
HdAiRenderDelegate::CreateSprim(TfToken const& typeId,
                                SdfPath const& sprimId) {
    if (typeId == HdPrimTypeTokens->camera) {
        return nullptr;
    } else {
        TF_CODING_ERROR("Unknown Sprim Type %s", typeId.GetText());
    }

    return nullptr;
}

HdSprim *
HdAiRenderDelegate::CreateFallbackSprim(TfToken const& typeId) {
    if (typeId == HdPrimTypeTokens->camera) {
        return nullptr;
    } else {
        TF_CODING_ERROR("Unknown Sprim Type %s", typeId.GetText());
    }

    return nullptr;
}

void
HdAiRenderDelegate::DestroySprim(HdSprim *sPrim) {
    delete sPrim;
}

HdBprim *
HdAiRenderDelegate::CreateBprim(TfToken const& typeId,
                                    SdfPath const& bprimId) {
    TF_CODING_ERROR("Unknown Bprim Type %s", typeId.GetText());
    return nullptr;
}

HdBprim *
HdAiRenderDelegate::CreateFallbackBprim(TfToken const& typeId) {
    TF_CODING_ERROR("Unknown Bprim Type %s", typeId.GetText());
    return nullptr;
}

void
HdAiRenderDelegate::DestroyBprim(HdBprim *bPrim) {
    delete bPrim;
}

PXR_NAMESPACE_CLOSE_SCOPE
