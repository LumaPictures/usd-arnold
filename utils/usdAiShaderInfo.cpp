#include <ai.h>

#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>

#include <pxr/usd/usdAi/aiNodeAPI.h>
#include <pxr/usd/usdAi/aiShader.h>
#include <pxr/usd/usdAi/aiShaderExport.h>
#include <pxr/usd/usdAi/tokens.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

constexpr auto helpText = R"VOGON(
usage usdAiShaderInfo
        [-h] [--cout]
        [--usd FILENAME]
        [--meta DIR|FILE]
        [--load DIR]

Print Arnold node information into a USD file or to the standard output.

By default usdAiShaderInfo prints the information to cout and the paths in
ARNOLD_PLUGIN_PATH are always loaded.

optional arguments:
 --cout     Print the node information to the cout using ascii encoding.
 --usd      Output the node information into a USD file.
 --meta     Load metadata file or all the .mtd files in a directory.
 --load     Load the shaders from a directory.
)VOGON";

enum class StatResult { IS_DIR, IS_FILE, IS_UNSUPPORTED };

StatResult checkPath(const char* p) {
    struct stat s;
    if (stat(p, &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            return StatResult::IS_DIR;
        } else if (s.st_mode & S_IFREG) {
            return StatResult::IS_FILE;
        }
    }
    return StatResult::IS_UNSUPPORTED;
}

} // namespace

int main(int argc, char* argv[]) {
    const auto* startArg = argv;
    const auto* endArg = argv + argc;

    auto findFlag = [&](const std::string& argName) -> bool {
        return std::find(startArg, endArg, argName) != endArg;
    };

    auto getFlagValue = [&](const std::string& argName,
                            const std::string& defaultValue =
                                "") -> std::string {
        const auto* flag = std::find(startArg, endArg, argName);
        if (flag >= (endArg - 1)) { return defaultValue; }
        return *(++flag);
    };

    auto getFlagValues =
        [&](const std::string& argName) -> std::vector<std::string> {
        std::vector<std::string> ret;
        const auto lastAllowed = endArg - 1;
        for (auto* flag = std::find(startArg, endArg, argName);
             flag < lastAllowed; flag = std::find(flag, endArg, argName)) {
            ret.emplace_back(*(++flag));
        }
        return ret;
    };

    if (findFlag("-h") || findFlag("--help")) {
        std::cout << helpText;
        return 0;
    }

    auto cout = findFlag("--cout");
    const std::string outFile = cout ? "" : getFlagValue("--usd", "");
    if (outFile.empty()) { cout = true; }

    auto stage = cout ? UsdStage::CreateInMemory(".usda")
                      : UsdStage::Open(SdfLayer::CreateNew(outFile));

    AiBegin();
    AiMsgSetConsoleFlags(AI_LOG_NONE);
    const auto* arnoldPluginPath = getenv("ARNOLD_PLUGIN_PATH");
    if (arnoldPluginPath != nullptr) { AiLoadPlugins(arnoldPluginPath); }

    for (const auto& plugin : getFlagValues("--load")) {
        AiLoadPlugins(plugin.c_str());
    }

    // TODO: check for the file extension.
    for (const auto& plugin : getFlagValues("--meta")) {
        const auto result = checkPath(plugin.c_str());
        if (result == StatResult::IS_DIR) {
            auto* d = opendir(plugin.c_str());
            if (d != nullptr) {
                for (struct dirent* de = nullptr;
                     (de = readdir(d)) != nullptr;) {
                    if (strcmp(de->d_name, ".") == 0) { continue; }
                    if (strcmp(de->d_name, "..") == 0) { continue; }
                    std::stringstream ss;
                    ss << plugin << "/" << de->d_name;
                    if (checkPath(ss.str().c_str()) == StatResult::IS_FILE) {
                        AiMetaDataLoadFile(ss.str().c_str());
                    }
                }
            }
        } else if (result == StatResult::IS_FILE) {
            AiMetaDataLoadFile(plugin.c_str());
        }
    }

    auto* nentryIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);

    while (!AiNodeEntryIteratorFinished(nentryIter)) {
        const auto* nentry = AiNodeEntryIteratorGetNext(nentryIter);
        const auto filename = AiNodeEntryGetFilename(nentry);
        const std::string nodeName = AiNodeEntryGetName(nentry);
        auto prim = stage->DefinePrim(SdfPath("/" + nodeName));
        prim.SetMetadata(
            UsdAiTokens->filename,
            VtValue(TfToken(filename == nullptr ? "<built-in>" : filename)));
        UsdAiShader shaderAPI(prim);
        shaderAPI.CreateIdAttr().Set(TfToken(nodeName));

        const auto nodeType = AiNodeEntryGetType(nentry);

        UsdAiNodeAPI nodeAPI(prim);
        nodeAPI.CreateNodeEntryTypeAttr().Set(
            UsdAiNodeAPI::GetNodeEntryTokenFromType(nodeType));

        auto paramIter = AiNodeEntryGetParamIterator(nentry);

        while (!AiParamIteratorFinished(paramIter)) {
            const auto* pentry = AiParamIteratorGetNext(paramIter);
            const auto paramType = AiParamGetType(pentry);

            UsdAttribute attr;
            if (paramType == AI_TYPE_ARRAY) {
                const auto* defaultValue = AiParamGetDefault(pentry);
                if (defaultValue == nullptr) { continue; }
                const auto* array = defaultValue->ARRAY();
                if (array == nullptr) { continue; }
                const auto elemType = AiArrayGetType(array);
                const auto* conversion =
                    AiShaderExport::get_array_conversion(elemType);
                if (conversion == nullptr) { continue; }
                attr = prim.CreateAttribute(
                    TfToken(AiParamGetName(pentry).c_str()), conversion->type,
                    false);

                if (conversion->f != nullptr) {
                    attr.Set(conversion->f(array));
                }
                attr.SetMetadata(
                    UsdAiTokens->elemType,
                    UsdAiNodeAPI::GetParamTypeTokenFromType(elemType));
            } else {
                const auto* conversion =
                    AiShaderExport::get_default_value_conversion(paramType);
                if (conversion == nullptr) { continue; }
                attr = prim.CreateAttribute(
                    TfToken(AiParamGetName(pentry).c_str()), conversion->type,
                    false);

                if (conversion->f != nullptr) {
                    attr.Set(conversion->f(*AiParamGetDefault(pentry), pentry));
                }
            }

            attr.SetMetadata(
                UsdAiTokens->paramType,
                UsdAiNodeAPI::GetParamTypeTokenFromType(paramType));

            auto* metaIter =
                AiNodeEntryGetMetaDataIterator(nentry, AiParamGetName(pentry));

            while (!AiMetaDataIteratorFinished(metaIter)) {
                nodeAPI.AddMetadataToAttribute(
                    attr, AiMetaDataIteratorGetNext(metaIter));
            }

            AiMetaDataIteratorDestroy(metaIter);
        }

        AiParamIteratorDestroy(paramIter);
    }

    AiNodeEntryIteratorDestroy(nentryIter);

    AiEnd();

    if (cout) {
        std::string out;
        stage->GetRootLayer()->ExportToString(&out);
        std::cout << out;
    } else {
        stage->GetRootLayer()->Save();
    }

    stage = UsdStageRefPtr();

    return 0;
}
