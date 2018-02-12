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

add_library(${PLUGIN_NAME} MODULE ${SRC} ${HDR})
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
set_target_properties(${PLUGIN_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)
target_include_directories(${PLUGIN_NAME} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_ARNOLD_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${USD_HOUDINI_INCLUDE_DIR})
target_include_directories(${PLUGIN_NAME} PRIVATE ${HOUDINI_INCLUDE_DIRS})
target_link_libraries(${PLUGIN_NAME} PRIVATE dl tf gf sdf usd usdGeom ${USD_ARNOLD_LIBRARY} gusd)
target_compile_definitions(${PLUGIN_NAME} PRIVATE
                           _GNU_SOURCE LINUX AMD64 SIZEOF_VOID_P=8
                           FBX_ENABLED=1 OPENCL_ENABLED=1 OPENVDB_ENABLED=1
                           MAKING_DSO SESI_LITTLE_ENDIAN ENABLE_THREADS USE_PTHREADS
                           _REENTRANT _FILE_OFFSET_BITS=64 GCC4 GCC3)
target_compile_definitions(${PLUGIN_NAME} PRIVATE
                           HOUDINI_API_VERSION=${HOUDINI_MAJOR_VERSION}${HOUDINI_MINOR_VERSION}
                           VERSION=${HOUDINI_VERSION}
                           HOUDINI_DOT_VERSION=${HOUDINI_BUILD_VERSION})
target_compile_options(${PLUGIN_NAME} PRIVATE
                       -fpermissive -Wall -Wno-parentheses -Wno-sign-compare
                       -Wno-reorder -Wno-uninitialized -Wunused -Wno-unused-parameter
                       -Wno-unused-local-typedefs -Wno-empty-body -fno-strict-aliasing)

add_subdirectory(scripts)

install(TARGETS ${PLUGIN_NAME}
        DESTINATION plugin)