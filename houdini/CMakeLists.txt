set(PLUGIN_NAME usd_arnold_houdini)

find_package(Houdini REQUIRED)
find_package(OpenEXR REQUIRED)
find_package(Boost REQUIRED)
find_package(Arnold REQUIRED)
find_package(USDArnold REQUIRED)

link_directories(${USD_HOUDINI_LIBRARY_DIR})
link_directories(${USD_LIBRARY_DIR})
link_directories(${HOUDINI_LIB_DIRS})

file(GLOB SRC *.cpp)
file(GLOB HDR *.h)

# Taken from USD. TODO: rewrite it into target_compile_options

add_definitions(
    -DHOUDINI_API_VERSION=${HOUDINI_MAJOR_VERSION}${HOUDINI_MINOR_VERSION}
    -DVERSION="${HOUDINI_VERSION}"
    -DHOUDINI_DOT_VERSION=${HOUDINI_BUILD_VERSION} )

add_definitions(
    -fpermissive -D_GNU_SOURCE -DLINUX -DAMD64 -DSIZEOF_VOID_P=8 -DFBX_ENABLED=1 -DOPENCL_ENABLED=1 -DOPENVDB_ENABLED=1
    -DMAKING_DSO -DSESI_LITTLE_ENDIAN -DENABLE_THREADS -DUSE_PTHREADS -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DGCC4 -DGCC3
    -Wall -Wno-parentheses -Wno-sign-compare -Wno-reorder -Wno-uninitialized -Wunused -Wno-unused-parameter -Wno-unused-local-typedefs -Wno-empty-body -fno-strict-aliasing )

add_library(${PLUGIN_NAME} MODULE ${SRC} ${HDR})
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
set_target_properties(${PLUGIN_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)
target_include_directories(${PLUGIN_NAME} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_ARNOLD_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_HOUDINI_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${HOUDINI_INCLUDE_DIRS})
target_link_libraries(${PLUGIN_NAME} PRIVATE dl tf gf sdf usd usdGeom ${USD_ARNOLD_LIBRARY} gusd)

add_subdirectory(scripts)

install(TARGETS ${PLUGIN_NAME}
        DESTINATION plugin)