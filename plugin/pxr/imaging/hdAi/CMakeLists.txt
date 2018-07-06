set(PXR_PREFIX pxr/imaging)
set(PXR_PACKAGE hdAi)

pxr_plugin(hdAi
    LIBRARIES
        plug
        tf
        vt
        gf
        work
        hf
        hd
        hdx
        usdAi
        ${TBB_LIBRARIES}
        ${ARNOLD_LIBRARY}

    INCLUDE_DIRS
        ${GLEW_INCLUDE_DIR}
        ${TBB_INCLUDE_DIRS}
        ${ARNOLD_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib

    PUBLIC_CLASSES
   #    config
   #    instancer
       mesh
   #    meshSamplers
   #    renderer
       rendererPlugin
       renderDelegate
       renderPass
   #    sampler

    #PUBLIC_HEADERS
   #    context.h
   #    renderParam.h

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