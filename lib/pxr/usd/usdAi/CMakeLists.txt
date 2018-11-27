
set(PXR_PREFIX pxr/usd)
set(PXR_PACKAGE usdAi)

# required for pxr_shared_library
if (NOT TARGET shared_libs)
    add_custom_target(shared_libs)
endif ()
if (NOT TARGET python)
    add_custom_target(python ALL)
endif ()

pxr_shared_library(usdAi
    LIBRARIES
        ${ARNOLD_LIBRARY}
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARY}
        tf
        gf
        vt
        sdf
        usd
        usdGeom
        usdShade

    INCLUDE_DIRS
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../

    PUBLIC_CLASSES
        tokens
        aiAOV
        aiDriver
        aiFilter
        aiLightAPI
        aiMaterialAPI
        aiNodeAPI
        aiProcedural
        aiProceduralNode
        aiShader
        aiShaderExport
        aiShapeAPI
        aiVolume
        aiVolumeAPI
        utils

    PUBLIC_HEADERS
        api.h
        rayTypes.h

    CPPFILES
        moduleDeps.cpp

    PYMODULE_CPPFILES
        module.cpp
        wrapAiAOV.cpp
        wrapAiDriver.cpp
        wrapAiFilter.cpp
        wrapAiLightAPI.cpp
        wrapAiMaterialAPI.cpp
        wrapAiNodeAPI.cpp
        wrapAiProcedural.cpp
        wrapAiProceduralNode.cpp
        wrapAiShader.cpp
        wrapAiShaderExport.cpp
        wrapAiShapeAPI.cpp
        wrapAiVolume.cpp
        wrapAiVolumeAPI.cpp
        wrapTokens.cpp

    PYTHON_FILES
        __init__.py

    RESOURCE_FILES
        plugInfo.json
        generatedSchema.usda
)

if (PXR_BUILD_TESTS)
    pxr_build_test(testUsdAiShapeAPI
       LIBRARIES
           usd
           ${PYTHON_LIBRARIES}
           arch
           usdAi
           ${GTEST_LIBRARY}
       INCLUDES
           ${GTEST_INCLUDE_DIR}
       CPPFILES
           testenv/testUsdAiShapeAPI.cpp
           testenv/testMain.cpp
    )

    pxr_build_test(testUsdAiNodeAPI
        LIBRARIES
            usd
            ${PYTHON_LIBRARIES}
            arch
            usdAi
            ${GTEST_LIBRARY}
        INCLUDES
            ${GTEST_INCLUDE_DIR}
        CPPFILES
            testenv/testUsdAiNodeAPI.cpp
        testenv/testMain.cpp
    )

    pxr_build_test(testUsdAiShaderExport
        LIBRARIES
            usd
            ${PYTHON_LIBRARIES}
            arch
            usdAi
            ${GTEST_LIBRARY}
        INCLUDES
            ${GTEST_INCLUDE_DIR}
        CPPFILES
            testenv/testUsdAiShaderExport.cpp
            testenv/testMain.cpp
    )

    pxr_register_test(testUsdAiShapeAPI
        COMMAND "${CMAKE_INSTALL_PREFIX}/tests/testUsdAiShapeAPI"
        EXPECTED_RETURN_CODE 0
    )

    pxr_register_test(testUsdAiNodeAPI
        COMMAND "${CMAKE_INSTALL_PREFIX}/tests/testUsdAiNodeAPI"
        EXPECTED_RETURN_CODE 0
    )

    pxr_register_test(testUsdAiShaderExport
        COMMAND "${CMAKE_INSTALL_PREFIX}/tests/testUsdAiShaderExport"
        EXPECTED_RETURN_CODE 0
        ENV
            USD_SHADE_WRITE_NEW_ENCODING=1
    )
endif ()

if (PXR_ENABLE_PYTHON_SUPPORT)
    install(CODE
    "file(WRITE \"${CMAKE_INSTALL_PREFIX}/lib/python/pxr/__init__.py\"
\"try:
    __import__('pkg_resources').declare_namespace(__name__)
except:
    from pkgutil import extend_path
    __path__ = extend_path(__path__, __name__)
\")")
endif (PXR_ENABLE_PYTHON_SUPPORT)

install(
    CODE
    "FILE(WRITE \"${CMAKE_INSTALL_PREFIX}/share/usd/plugins/plugInfo.json\"
    \"
{
    \\\"Includes\\\": [ \\\"*/resources/\\\" ]
}
    \")")
