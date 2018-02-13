#include <ai.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

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

int main(int argc, char* argv[]) {
    const auto* startArg = argv;
    const auto* endArg = argv + argc;

    auto findFlag = [&](const std::string& argName) -> bool {
        return std::find(startArg, endArg, argName) != endArg;
    };

    auto getFlagValue = [&](const std::string& argName,
                            const std::string& defaultValue = "") -> std::string {
        const auto* flag = std::find(startArg, endArg, argName);
        if (flag >= (endArg - 1)) { return defaultValue; }
        return *(++flag);
    };

    auto getFlagValues = [&](const std::string& argName) -> std::vector<std::string> {
        std::vector<std::string> ret;
        const auto lastAllowed = endArg - 1;
        for (auto* flag = std::find(startArg, endArg, argName);
             flag < lastAllowed;
             flag = std::find(flag, endArg, argName)) {
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

    AiBegin();
    AiMsgSetConsoleFlags(AI_LOG_NONE);
    const auto* arnoldPluginPath = getenv("ARNOLD_PLUGIN_PATH");
    if (arnoldPluginPath != nullptr) { AiLoadPlugins(arnoldPluginPath); }

    for (const auto& plugin: getFlagValues("--load")) {
        AiLoadPlugins(plugin.c_str());
    }

    auto* nentryIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);

    while (!AiNodeEntryIteratorFinished(nentryIter)) {
        const auto* nentry = AiNodeEntryIteratorGetNext(nentryIter);

        auto* metaIter = AiNodeEntryGetMetaDataIterator(nentry);

        while (!AiMetaDataIteratorFinished(metaIter)) {

        }

        AiMetaDataIteratorDestroy(metaIter);

        auto paramIter = AiNodeEntryGetParamIterator(nentry);

        while (!AiParamIteratorFinished(paramIter)) {
            const auto* pentry = AiParamIteratorGetNext(paramIter);

            metaIter = AiNodeEntryGetMetaDataIterator(nentry, AiParamGetName(pentry));

            while (!AiMetaDataIteratorFinished(metaIter)) {

            }

            AiMetaDataIteratorDestroy(metaIter);
        }

        AiParamIteratorDestroy(paramIter);
    }

    AiNodeEntryIteratorDestroy(nentryIter);

    AiEnd();

    return 0;
}
