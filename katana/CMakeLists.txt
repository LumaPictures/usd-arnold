set(PLUGIN_NAME usd_arnold_katana)

find_package(Katana REQUIRED)
find_package(OpenEXR REQUIRED)
find_package(Boost REQUIRED)
find_package(Arnold REQUIRED)
find_package(USDArnold REQUIRED)

link_directories(${USD_KATANA_LIBRARY_DIR})

file(GLOB FNATTRIBUTE_SRC ${KATANA_API_SOURCE_DIR}/FnAttribute/client/*.cpp)
file(GLOB FNGEOLIBOP_SRC ${KATANA_API_SOURCE_DIR}/FnGeolib/op/*.cpp)
file(GLOB FNGEOLIBUTIL_SRC ${KATANA_API_SOURCE_DIR}/FnGeolib/util/*.cpp)
file(GLOB FNGEOLIBSERVICES_SRC ${KATANA_API_SOURCE_DIR}/FnGeolibServices/client/*.cpp)
file(GLOB FNPLUGINMANAGER_SRC ${KATANA_API_SOURCE_DIR}/FnPluginManager/client/*.cpp)
file(GLOB FNPLUGINSYSTEM_SRC ${KATANA_API_SOURCE_DIR}/FnPluginSystem/*.cpp)
file(GLOB FNLOGGING_SRC ${KATANA_API_SOURCE_DIR}/FnLogging/client/*.cpp)
file(GLOB PYSTRING_SRC ${KATANA_API_SOURCE_DIR}/pystring/*.cpp)

link_directories(${KATANA_API_BASE_DIR}/bin/Geolib3/internal/CEL)

file(GLOB SRC *.cpp)
set(API_SRC
    ${FNATTRIBUTE_SRC}
    ${FNGEOLIBOP_SRC}
    ${FNGEOLIBUTIL_SRC}
    ${FNGEOLIBSERVICES_SRC}
    ${FNPLUGINMANAGER_SRC}
    ${FNPLUGINSYSTEM_SRC}
    ${FNLOGGING_SRC}
    ${PYSTRING_SRC})
add_library(${PLUGIN_NAME} MODULE ${SRC} ${API_SRC})
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
set_target_properties(${PLUGIN_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)
target_include_directories(${PLUGIN_NAME} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_ARNOLD_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${KATANA_API_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_KATANA_INCLUDE_DIR})
target_link_libraries(${PLUGIN_NAME} CEL ${OPENEXR_LIBRARIES} dl tf gf sdf usd usdGeom ${USD_ARNOLD_LIBRARY} usdKatana)

if (ARNOLD_VERSION_ARCH_NUM VERSION_GREATER "4")
    target_compile_definitions(${PLUGIN_NAME} PRIVATE "-DARNOLD5")
endif ()

install(TARGETS ${PLUGIN_NAME}
        DESTINATION Ops)
