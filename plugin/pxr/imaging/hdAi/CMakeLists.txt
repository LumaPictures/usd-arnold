set(PXR_PREFIX pxr/imaging)
set(PXR_PACKAGE hdAi)

# required for pxr_shared_library
if (NOT TARGET shared_libs)
    add_custom_target(shared_libs)
endif ()
if (NOT TARGET python)
add_custom_target(python ALL)
endif ()

find_package(PythonLibs REQUIRED)

pxr_plugin(hdAi
    LIBRARIES
        ${ARNOLD_LIBRARY}
        ${PYTHON_LIBRARY}
        ${Boost_LIBRARIES}
        plug
        tf
        vt
        gf
        work
        hf
        hd
        hdx
        sdf
        ${TBB_LIBRARIES}

    INCLUDE_DIRS
        ${Boost_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIR}
        ${TBB_INCLUDE_DIRS}
        ${ARNOLD_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib
        ${PYTHON_INCLUDE_DIRS}

    PUBLIC_CLASSES
       mesh
       rendererPlugin
       renderBuffer
       renderDelegate
       renderPass
       utils

   CPPFILES
       nodes/camera.cpp
       nodes/driver.cpp
       nodes/nodes.cpp

   PRIVATE_HEADERS
       nodes/nodes.h

    RESOURCE_FILES
        plugInfo.json
)

install(
    CODE
    "FILE(WRITE \"${CMAKE_INSTALL_PREFIX}/plugin/usd/plugInfo.json\"
    \"
{
    \\\"Includes\\\": [ \\\"*/resources/\\\" ]
}
    \")")