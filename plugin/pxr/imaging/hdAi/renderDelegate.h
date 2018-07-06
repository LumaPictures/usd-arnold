#ifndef HDAI_RENDER_DELEGATE_H
#define HDAI_RENDER_DELEGATE_H

#include <pxr/pxr.h>
#include <pxr/imaging/hd/renderDelegate.h>
#include <pxr/imaging/hd/renderThread.h>
#include <pxr/imaging/hd/resourceRegistry.h>

PXR_NAMESPACE_OPEN_SCOPE

class HdAiRenderDelegate final : public HdRenderDelegate {
public:
    HdAiRenderDelegate();

    ~HdAiRenderDelegate() override;

    HdRenderParam* GetRenderParam() const override;

    const TfTokenVector& GetSupportedRprimTypes() const override;

    const TfTokenVector& GetSupportedSprimTypes() const override;

    const TfTokenVector& GetSupportedBprimTypes() const override;

    HdResourceRegistrySharedPtr GetResourceRegistry() const override;

    HdRenderPassSharedPtr CreateRenderPass(HdRenderIndex* index,
                                           HdRprimCollection const& collection) override;

    HdInstancer* CreateInstancer(HdSceneDelegate* delegate,
                                 SdfPath const& id,
                                 SdfPath const& instancerId) override;

    void DestroyInstancer(HdInstancer* instancer) override;

    HdRprim* CreateRprim(TfToken const& typeId,
                         SdfPath const& rprimId,
                         SdfPath const& instancerId) override;

    void DestroyRprim(HdRprim* rPrim) override;

    HdSprim* CreateSprim(TfToken const& typeId,
                         SdfPath const& sprimId) override;

    HdSprim* CreateFallbackSprim(TfToken const& typeId) override;

    void DestroySprim(HdSprim* sPrim) override;

    HdBprim* CreateBprim(TfToken const& typeId,
                         SdfPath const& bprimId) override;

    HdBprim* CreateFallbackBprim(TfToken const& typeId) override;

    void DestroyBprim(HdBprim* bPrim) override;

    void CommitResources(HdChangeTracker* tracker) override;

private:
    static const TfTokenVector SUPPORTED_RPRIM_TYPES;
    static const TfTokenVector SUPPORTED_SPRIM_TYPES;
    static const TfTokenVector SUPPORTED_BPRIM_TYPES;

    static std::mutex _mutexResourceRegistry;
    static std::atomic_int _counterResourceRegistry;
    static HdResourceRegistrySharedPtr _resourceRegistry;

    HdAiRenderDelegate(const HdAiRenderDelegate&) = delete;

    HdAiRenderDelegate& operator=(const HdAiRenderDelegate&) = delete;
};


PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDAI_RENDER_DELEGATE_H
