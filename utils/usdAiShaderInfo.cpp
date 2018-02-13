#include <ai.h>

#include <string>
#include <algorithm>
#include <iostream>

constexpr auto helpText = R"VOGON(
usage usdAiShaderInfo
        [-h] [--cout]
        [--usd FILENAME]
        [--meta DIR|FILE]
        [--load DIR|FILE]

Print Arnold node information into a USD file or to the standard output.

By default usdAiShaderInfo prints the information to cout and the paths in
ARNOLD_PLUGIN_PATH are always loaded.

optional arguments:
 -c, --cout     Print the node information to the cout using ascii encoding.
 -u, --usd      Output the node information into a USD file.
 -m, --meta     Load metadata file or all the .mtd files in a directory.
 -l, --load     Load the shader file or all the shaders from the directory.
)VOGON";

int main(int argc, char* argv[]) {
    const auto* startArg = argv;
    const auto* endArg = argv + argc;

    auto findFlag = [&](const std::string& argName) -> bool {
        return std::find(startArg, endArg, argName) != endArg;
    };

    if (findFlag("-h") || findFlag("--help")) {
        std::cout << helpText;
        return 0;
    }

    return 0;
}
