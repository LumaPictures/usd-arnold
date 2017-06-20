set(PLUGIN_NAME arnoldShaderExporter)

include_directories(${USD_ROOT}/third_party/maya/include)
link_directories(${USD_ROOT}/third_party/maya/lib)

find_package(Maya)
include_directories(${MAYA_INCLUDE_DIRS})
link_directories(${MAYA_LIBRARY_DIR})

find_package(Arnold)
include_directories(${ARNOLD_INCLUDE_DIR})

set(MAYA_LIBS OpenMaya Foundation OpenMayaAnim OpenMayaFX OpenMayaUI OpenMayaRender)

file(GLOB SRC *.cpp)

find_package(Boost COMPONENTS python REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_library(${PLUGIN_NAME} SHARED ${SRC})
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
target_link_libraries(${PLUGIN_NAME} ${ARNOLD_LIBRARY} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${MAYA_LIBS} tf gf sdf usd usdGeom usdMaya)

install(TARGETS ${PLUGIN_NAME}
        DESTINATION plug-ins)

set(PLUGINFO_OUT ${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json)
configure_file(plugInfo.json.in ${PLUGINFO_OUT})

install(FILES ${PLUGINFO_OUT}
        DESTINATION .)
