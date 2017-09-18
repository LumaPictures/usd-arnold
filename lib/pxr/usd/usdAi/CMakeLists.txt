
set(PXR_PREFIX pxr/usd)
set(PXR_PACKAGE usdAi)

# required for pxr_shared_library
add_custom_target(shared_libs)
add_custom_target(python ALL)

pxr_shared_library(usdAi
    LIBRARIES
        ${ARNOLD_LIBRARY}
        ${Boost_LIBRARIES}
        tf
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
        aiMaterialAPI
        aiNodeAPI
        aiProcedural
        aiShader
        aiShaderExport
        aiShapeAPI
        aiVolume

    PUBLIC_HEADERS
        api.h
        rayTypes4.h
        rayTypes5.h

    CPPFILES
        moduleDeps.cpp

    PYMODULE_CPPFILES
        module.cpp
        wrapAiMaterialAPI.cpp
        wrapAiNodeAPI.cpp
        wrapAiProcedural.cpp
        wrapAiShader.cpp
        wrapAiShaderExport.cpp
        wrapAiShapeAPI.cpp
        wrapAiVolume.cpp
        wrapTokens.cpp

    PYTHON_FILES
        __init__.py

    RESOURCE_FILES
        plugInfo.json
        generatedSchema.usda
)

if (PXR_ENABLE_PYTHON_SUPPORT)
    install(CODE
    "file(WRITE \"${CMAKE_INSTALL_PREFIX}/lib/python/pxr/__init__.py\"
\"try:
    __import__('pkg_resources').declare_namespace(__name__)
except:
    from pkgutil import extend_path
    __path__ = extend_path(__path__, __name__)
\")"
    )
endif (PXR_ENABLE_PYTHON_SUPPORT)