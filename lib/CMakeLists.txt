set(LIBRARY_NAME usdAi)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS python REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB AI_SRC ai*.cpp)

add_library(${LIBRARY_NAME} SHARED ${AI_SRC} tokens.cpp)
target_link_libraries(${LIBRARY_NAME} tf vt sdf usd usdGeom usdShade)

set(BUILTIN_SCHEMA_DIR ${USD_INCLUDE_DIR}/pxr/usd)

set(CONFIGURED_SCHEMA ${CMAKE_CURRENT_BINARY_DIR}/schema.usda)

configure_file(schema.usda.in ${CONFIGURED_SCHEMA})

add_custom_command(
    TARGET ${LIBRARY_NAME} PRE_BUILD
    COMMAND usdGenSchema ${CONFIGURED_SCHEMA} ${CMAKE_CURRENT_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
