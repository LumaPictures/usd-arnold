set(PLUGIN_NAME arnoldForUsdMaya)

find_package(Maya)
find_package(USDArnold REQUIRED)

link_directories(${USD_MAYA_LIBRARY_DIR})
link_directories(${MAYA_LIBRARY_DIR})

set(MAYA_LIBS OpenMaya Foundation OpenMayaAnim OpenMayaFX OpenMayaUI OpenMayaRender)

file(GLOB SRC *.cpp)

find_package(Boost COMPONENTS python REQUIRED)

add_library(${PLUGIN_NAME} SHARED ${SRC})
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
target_link_libraries(${PLUGIN_NAME}
                      ${Boost_LIBRARIES}
                      ${PYTHON_LIBRARIES} ${MAYA_LIBS}
                      tf gf sdf usd usdGeom ${USD_ARNOLD_LIBRARY} usdMaya)
set_target_properties(${PLUGIN_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)
target_include_directories(${PLUGIN_NAME} PRIVATE SYSTEM ${Boost_INCLUDE_DIRS})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_MAYA_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${MAYA_INCLUDE_DIRS})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_ARNOLD_INCLUDE_DIR})
target_compile_options(${PLUGIN_NAME} PRIVATE -D_LINUX)

install(TARGETS ${PLUGIN_NAME}
        DESTINATION plug-ins)

set(PLUGINFO_OUT ${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json)
configure_file(plugInfo.json.in ${PLUGINFO_OUT})

install(FILES ${PLUGINFO_OUT}
        DESTINATION .)
