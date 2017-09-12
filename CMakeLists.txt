project(usdai)

option(BUILD_USD_PLUGIN "Building the usd plugin." OFF)
option(BUILD_USD_MAYA_PLUGIN "Building the usd maya plugin." OFF)
option(BUILD_USD_KATANA_PLUGIN "Building the usd katana plugin." OFF)
option(ENABLE_PYTHON_SUPPORT "Enable Python based components for USD" ON)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options(-Wall -DBUILD_OPTLEVEL_OPT -DBUILD_COMPONENT_SRC_PREFIX="")
set(CMAKE_CXX_STANDARD 11)

find_package(USD REQUIRED)
find_package(TBB REQUIRED)

include_directories(${USD_INCLUDE_DIR})
link_directories(${USD_LIBRARY_DIR})

include_directories(SYSTEM ${TBB_INCLUDE_DIRS})

if (ENABLE_PYTHON_SUPPORT)
    add_definitions(-DPXR_PYTHON_SUPPORT_ENABLED)
    find_package(PythonLibs REQUIRED)
    include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS})
#    find_package(Boost COMPONENTS
#                    python
#                    REQUIRED
#    )
#    include_directories(${Boost_INCLUDE_DIRS})
endif ()

if (BUILD_USD_PLUGIN)
    add_subdirectory(lib/pxr/usd/usdAi)
endif ()

if (BUILD_USD_MAYA_PLUGIN)
    add_subdirectory(usdMaya)
endif ()

if (BUILD_USD_KATANA_PLUGIN)
    add_subdirectory(katana)
endif ()

install(FILES README.md DESTINATION .)
install(FILES LICENSE.md DESTINATION .)
