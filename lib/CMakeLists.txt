set(LIBRARY_NAME usdAi)
set(PYTHON_LIBRARY_NAME _${LIBRARY_NAME})

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS python REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB AI_SRC ai*.cpp)
file(GLOB HDR *.h)
file(GLOB WRAP_SRC wrap*.cpp)
set(PY_SRC 
    module.cpp
    moduleDeps.cpp
    ${WRAP_SRC})

set(BUILTIN_SCHEMA_DIR ${USD_INCLUDE_DIR}/pxr/usd)
set(CONFIGURED_SCHEMA ${CMAKE_CURRENT_BINARY_DIR}/schema.usda)
configure_file(schema.usda.in ${CONFIGURED_SCHEMA})

######################################
# Installing and building c++ library.

# This is always going to run. TODO: try using add_custom_command differently.
add_custom_target(schema_gen
    COMMAND usdGenSchema ${CONFIGURED_SCHEMA} ${CMAKE_CURRENT_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES schema.usda.in)

add_library(${LIBRARY_NAME} SHARED ${AI_SRC} tokens.cpp ${HDR})
target_link_libraries(${LIBRARY_NAME} ${Boost_LIBRARIES} tf vt sdf usd usdGeom usdShade)
set_target_properties(${LIBRARY_NAME}
    PROPERTIES COMPILE_DEFINITIONS 
        "MFB_PACKAGE_NAME=${LIBRARY_NAME};MFB_ALT_PACKAGE_NAME=${LIBRARY_NAME};MFB_PACKAGE_MODULE=${LIBRARY_NAME}"
)

add_dependencies(${LIBRARY_NAME} schema_gen)

install(TARGETS ${LIBRARY_NAME}
    DESTINATION lib)

install(FILES generatedSchema.usda
    DESTINATION share/usd/plugins/usdAi/resources)

install(CODE
    "file(WRITE \"${CMAKE_INSTALL_PREFIX}/share/usd/plugins/plugInfo.json\" \"{\n    \\\"Includes\\\": [ \\\"*/resources/\\\" ]\n}\")")

set(PLUG_INFO_LIBRARY_PATH "../../../../lib/lib${LIBRARY_NAME}.so")
set(PLUG_INFO_ROOT "..")
set(PLUG_INFO_RESOURCE_PATH resources)
set(CONFIGURED_PLUGINFO "${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json")
configure_file(plugInfo.json ${CONFIGURED_PLUGINFO})
install(FILES ${CONFIGURED_PLUGINFO}
    DESTINATION share/usd/plugins/usdAi/resources)

install(FILES ${HDR}
    DESTINATION include/${LIBRARY_NAME})

##########################################
# Installing and building python bindings.

# So the python lib links against the right one.
set_target_properties(${LIBRARY_NAME} PROPERTIES LIBRARY_LOCATION2 ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

add_library(${PYTHON_LIBRARY_NAME} SHARED ${PY_SRC})
set_target_properties(${PYTHON_LIBRARY_NAME} PROPERTIES PREFIX "")
set_target_properties(${PYTHON_LIBRARY_NAME}
    PROPERTIES COMPILE_DEFINITIONS 
        "MFB_PACKAGE_NAME=${LIBRARY_NAME};MFB_ALT_PACKAGE_NAME=${LIBRARY_NAME};MFB_PACKAGE_MODULE=${LIBRARY_NAME}")

target_link_libraries(${PYTHON_LIBRARY_NAME} ${LIBRARY_NAME} tf sdf usd vt ${Boost_LIBRARIES})
install(TARGETS ${PYTHON_LIBRARY_NAME}
    DESTINATION lib/python/usd/${LIBRARY_NAME})

install(CODE
    "file(WRITE \"${CMAKE_INSTALL_PREFIX}/lib/python/usd/__init__.py\" \"try:\n\t__import__('pkg_resources').declare_namespace(__name__)\nexcept:\n\tfrom pkgutil import extend_path\n\t__path__ = extend_path(__path__, __name__)\n\")"
)

install(CODE
    "file(WRITE \"${CMAKE_INSTALL_PREFIX}/lib/python/usd/${LIBRARY_NAME}/__init__.py\" \"import ${PYTHON_LIBRARY_NAME}\nfrom pxr import Tf\nTf.PrepareModule(${PYTHON_LIBRARY_NAME}, locals())\ndel Tf\n\ntry:\n\t__import__('pkg_resources').declare_namespace(__name__)\nexcept:\n\tfrom pkgutil import extend_path\n\t__path__ = extend_path(__path__, __name__)\n\")"
)
