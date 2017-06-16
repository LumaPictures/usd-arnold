project(usdai)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options(-Wall -DBUILD_OPTLEVEL_OPT -DBUILD_COMPONENT_SRC_PREFIX="")
set(CMAKE_CXX_STANDARD 11)

find_package(USD REQUIRED)
find_package(TBB REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(${USD_INCLUDE_DIR})
link_directories(${USD_LIBRARY_DIR})

include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS})
include_directories(SYSTEM ${TBB_INCLUDE_DIRS})

add_subdirectory(lib)

install(FILES README.md DESTINATION .)
