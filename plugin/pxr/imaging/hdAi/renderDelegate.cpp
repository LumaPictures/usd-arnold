#include "pxr/imaging/hdAi/renderDelegate.h"

#include <pxr/imaging/glf/glew.h>
#include <pxr/imaging/hd/resourceRegistry.h>
#include <pxr/imaging/hd/camera.h>
#include <pxr/imaging/hd/bprim.h>
#include <pxr/imaging/hd/tokens.h>
#include <pxr/imaging/hd/instancer.h>
#include <pxr/imaging/hd/rprim.h>

#include <ai.h>

#include "pxr/imaging/hdAi/mesh.h"
#include "pxr/imaging/hdAi/renderBuffer.h"
#include "pxr/imaging/hdAi/renderPass.h"

PXR_NAMESPACE_OPEN_SCOPE

const TfTokenVector HdAiRenderDelegate::SUPPORTED_RPRIM_TYPES = {
    HdPrimTypeTokens->mesh,
};

const TfTokenVector HdAiRenderDelegate::SUPPORTED_SPRIM_TYPES = {
    HdPrimTypeTokens->camera,
};

const TfTokenVector HdAiRenderDelegate::SUPPORTED_BPRIM_TYPES = {
    HdPrimTypeTokens->renderBuffer,
};

std::mutex HdAiRenderDelegate::_mutexResourceRegistry;
std::atomic_int HdAiRenderDelegate::_counterResourceRegistry;
HdResourceRegistrySharedPtr HdAiRenderDelegate::_resourceRegistry;

HdAiRenderDelegate::HdAiRenderDelegate() {
    if (AiUniverseIsActive()) {
        TF_CODING_ERROR("There is already an active Arnold universe!");
    }
    AiBegin(AI_SESSION_INTERACTIVE);
    std::lock_guard<std::mutex> guard(_mutexResourceRegistry);
    if (_counterResourceRegistry.fetch_add(1) == 0) {
        _resourceRegistry.reset( new HdResourceRegistry() );
    }
    AiMsgSetConsoleFlags(AI_LOG_WARNINGS | AI_LOG_ERRORS);
}

HdAiRenderDelegate::~HdAiRenderDelegate() {
    std::lock_guard<std::mutex> guard(_mutexResourceRegistry);
    if (_counterResourceRegistry.fetch_sub(1) == 1) {
        _resourceRegistry.reset();
    }
    AiEnd();
}

HdRenderParam*
HdAiRenderDelegate::GetRenderParam() const {
    return nullptr;
}

void
HdAiRenderDelegate::CommitResources(HdChangeTracker* tracker) {
}

const TfTokenVector&
HdAiRenderDelegate::GetSupportedRprimTypes() const {
    return SUPPORTED_RPRIM_TYPES;
}

const TfTokenVector&
HdAiRenderDelegate::GetSupportedSprimTypes() const {
    return SUPPORTED_SPRIM_TYPES;
}

const TfTokenVector&
HdAiRenderDelegate::GetSupportedBprimTypes() const {
    return SUPPORTED_BPRIM_TYPES;
}

HdResourceRegistrySharedPtr
HdAiRenderDelegate::GetResourceRegistry() const {
    return _resourceRegistry;
}

HdRenderPassSharedPtr
HdAiRenderDelegate::CreateRenderPass(
    HdRenderIndex* index,
    const HdRprimCollection& collection) {
    return HdRenderPassSharedPtr(new HdAiRenderPass(index, collection));
}

HdInstancer*
HdAiRenderDelegate::CreateInstancer(
    HdSceneDelegate* delegate,
    const SdfPath& id,
    const SdfPath& instancerId) {
    return nullptr;
}

void
HdAiRenderDelegate::DestroyInstancer(HdInstancer* instancer) {
    delete instancer;
}

HdRprim*
HdAiRenderDelegate::CreateRprim(
    const TfToken& typeId,
    const SdfPath& rprimId,
    const SdfPath& instancerId) {
    if (typeId == HdPrimTypeTokens->mesh) {
        return nullptr;
    }
    TF_CODING_ERROR("Unknown Rprim Type %s", typeId.GetText());
    return nullptr;
}

void
HdAiRenderDelegate::DestroyRprim(HdRprim* rPrim) {
    delete rPrim;
}

HdSprim*
HdAiRenderDelegate::CreateSprim(
    const TfToken& typeId,
    const SdfPath& sprimId) {
    if (typeId == HdPrimTypeTokens->camera) {
        return new HdCamera(sprimId);
    }
    TF_CODING_ERROR("Unknown Sprim Type %s", typeId.GetText());
    return nullptr;
}

HdSprim*
HdAiRenderDelegate::CreateFallbackSprim(const TfToken& typeId) {
    if (typeId == HdPrimTypeTokens->camera) {
        return new HdCamera(SdfPath::EmptyPath());
    }
    TF_CODING_ERROR("Unknown Sprim Type %s", typeId.GetText());
    return nullptr;
}

void
HdAiRenderDelegate::DestroySprim(HdSprim* sPrim) {
    delete sPrim;
}

HdBprim*
HdAiRenderDelegate::CreateBprim(
    const TfToken& typeId,
    const SdfPath& bprimId) {
    if (typeId == HdPrimTypeTokens->renderBuffer) {
        return new HdAiRenderBuffer(bprimId);
    }
    TF_CODING_ERROR("Unknown Bprim Type %s", typeId.GetText());
    return nullptr;
}

HdBprim*
HdAiRenderDelegate::CreateFallbackBprim(const TfToken& typeId) {
    if (typeId == HdPrimTypeTokens->renderBuffer) {
        return new HdAiRenderBuffer(SdfPath());
    }
    TF_CODING_ERROR("Unknown Bprim Type %s", typeId.GetText());
    return nullptr;
}

void
HdAiRenderDelegate::DestroyBprim(HdBprim* bPrim) {
    delete bPrim;
}

PXR_NAMESPACE_CLOSE_SCOPE
