#include "pxr/pxr.h"
#include "pxr/base/tf/registryManager.h"
#include "pxr/base/tf/scriptModuleLoader.h"
#include "pxr/base/tf/token.h"

#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfScriptModuleLoader) {
    std::vector<TfToken> reqs;
    reqs.reserve(15);
    reqs.push_back(TfToken("ar"));
    reqs.push_back(TfToken("arch"));
    reqs.push_back(TfToken("gf"));
    reqs.push_back(TfToken("js"));
    reqs.push_back(TfToken("kind"));
    reqs.push_back(TfToken("pcp"));
    reqs.push_back(TfToken("plug"));
    reqs.push_back(TfToken("sdf"));
    reqs.push_back(TfToken("tf"));
    reqs.push_back(TfToken("tracelite"));
    reqs.push_back(TfToken("usd"));
    reqs.push_back(TfToken("usdGeom"));
    reqs.push_back(TfToken("usdShade"));
    reqs.push_back(TfToken("vt"));
    reqs.push_back(TfToken("work"));
    TfScriptModuleLoader::GetInstance().
        RegisterLibrary(TfToken("usdAi"), TfToken("pxr.UsdAi"), reqs);
}

PXR_NAMESPACE_CLOSE_SCOPE
