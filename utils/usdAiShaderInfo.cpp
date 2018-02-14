#include <ai.h>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>

#include <pxr/usd/usdAi/aiNodeAPI.h>
#include <pxr/usd/usdAi/aiShader.h>
#include <pxr/usd/usdAi/aiShaderExport.h>

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

uint8_t getEntryType(const AtParamEntry* e) {
    return AiParamGetType(e);
}

uint8_t getEntryType(const AtMetaDataEntry* e) {
    return e->type;
}

std::string getEntryName(const AtParamEntry* e) {
    return AiParamGetName(e).c_str();
}

std::string getEntryName(const AtMetaDataEntry* e) {
    return e->name.c_str();
}

VtValue getValue(const AtMetaDataEntry* e) {
    const auto t = getEntryType(e);
    if (t == AI_TYPE_BYTE) {
        return VtValue(e->value.BYTE());
    }
    return VtValue();
}

const std::unordered_map<int, TfToken> nodeTypeToToken {
    {AI_NODE_UNDEFINED, TfToken("undefined")},
    {AI_NODE_OPTIONS, TfToken("options")},
    {AI_NODE_CAMERA, TfToken("camera")},
    {AI_NODE_LIGHT, TfToken("light")},
    {AI_NODE_SHAPE, TfToken("shape")},
    {AI_NODE_SHADER, TfToken("shader")},
    {AI_NODE_OVERRIDE, TfToken("override")},
    {AI_NODE_DRIVER, TfToken("driver")},
    {AI_NODE_FILTER, TfToken("filter")},
    {AI_NODE_COLOR_MANAGER, TfToken("color_manager")},
    {AI_NODE_SHAPE_PROCEDURAL, TfToken("procedural")},
    {AI_NODE_SHAPE_VOLUME, TfToken("volume")},
    {AI_NODE_SHAPE_IMPLICIT, TfToken("implicit")},
    {AI_NODE_ALL, TfToken("all")},
};

TfToken getNodeTypeToken(int nt) {
    const auto it = nodeTypeToToken.find(nt);
    if (it != nodeTypeToToken.end()) {
        return it->second;
    } else {
        return TfToken("");
    }
}

const std::unordered_map<uint8_t, TfToken> paramTypeToToken{
    {AI_TYPE_BYTE,    TfToken("byte")},
    {AI_TYPE_INT,     TfToken("int")},
    {AI_TYPE_UINT,    TfToken("uint")},
    {AI_TYPE_BOOLEAN, TfToken("boolean")},
    {AI_TYPE_FLOAT,   TfToken("float")},
    {AI_TYPE_RGB,     TfToken("rgb")},
    {AI_TYPE_RGBA,    TfToken("rgba")},
    {AI_TYPE_VECTOR,  TfToken("vector")},
    {AI_TYPE_VECTOR2, TfToken("vector2")},
    {AI_TYPE_STRING,  TfToken("string")},
    {AI_TYPE_POINTER, TfToken("pointer")},
    {AI_TYPE_NODE, TfToken("node")},
    {AI_TYPE_ARRAY, TfToken("array")},
    {AI_TYPE_MATRIX, TfToken("matrix")},
    {AI_TYPE_ENUM, TfToken("enum")},
    {AI_TYPE_CLOSURE, TfToken("closure")},
    {AI_TYPE_USHORT, TfToken("ushort")},
    {AI_TYPE_HALF, TfToken("half")},
    {AI_TYPE_UNDEFINED, TfToken("undefined")},
    {AI_TYPE_NONE, TfToken("none")},
};

TfToken getParamTypeToken(int pt) {
    const auto it = paramTypeToToken.find(pt);
    if (it != paramTypeToToken.end()) {
        return it->second;
    } else {
        return TfToken("");
    }
}

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

}

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

    auto stage = cout ?
                 UsdStage::CreateInMemory(".usda") :
                 UsdStage::Open(SdfLayer::CreateNew(outFile));

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
        const std::string nodeName = AiNodeEntryGetName(nentry);
        auto prim = stage->DefinePrim(SdfPath("/" + nodeName));
        UsdAiShader shaderAPI(prim);
        shaderAPI.CreateIdAttr().Set(TfToken(nodeName));

        const auto nodeType = AiNodeEntryGetType(nentry);

        UsdAiNodeAPI nodeAPI(prim);
        nodeAPI.CreateNodeEntryTypeAttr().Set(getNodeTypeToken(nodeType));

        auto* metaIter = AiNodeEntryGetMetaDataIterator(nentry);

        while (!AiMetaDataIteratorFinished(metaIter)) {
            const auto* mentry = AiMetaDataIteratorGetNext(metaIter);
            const auto mentryType = getEntryType(mentry);
        }

        AiMetaDataIteratorDestroy(metaIter);

        auto paramIter = AiNodeEntryGetParamIterator(nentry);

        while (!AiParamIteratorFinished(paramIter)) {
            const auto* pentry = AiParamIteratorGetNext(paramIter);
            const auto pentryType = getEntryType(pentry);

            const auto* conversion = AiShaderExport::get_default_value_conversion(pentryType);
            if (conversion == nullptr) { continue; }
            auto attr = prim.CreateAttribute(
                TfToken(AiParamGetName(pentry).c_str()),
                conversion->type, false);

            if (conversion->f != nullptr) {
                attr.Set(conversion->f(pentry));
            }

            const static TfToken idToken("param_type");
            attr.SetMetadata(idToken, getParamTypeToken(pentryType));

            metaIter = AiNodeEntryGetMetaDataIterator(nentry, AiParamGetName(pentry));

            while (!AiMetaDataIteratorFinished(metaIter)) {
                const auto* mentry = AiMetaDataIteratorGetNext(metaIter);
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
