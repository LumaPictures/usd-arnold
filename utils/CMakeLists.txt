set(SHADER_INFO usdAiShaderInfo)

find_package(OpenEXR REQUIRED)
find_package(Boost REQUIRED COMPONENTS system Python)
find_package(Arnold REQUIRED)
find_package(PythonLibs REQUIRED)

add_executable(${SHADER_INFO} usdAiShaderInfo.cpp)
set_target_properties(${SHADER_INFO} PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)
set_target_properties(${SHADER_INFO} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
target_include_directories(${SHADER_INFO} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${SHADER_INFO} PRIVATE ${USD_INCLUDE_DIR})
target_include_directories(${SHADER_INFO} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}../lib)
target_link_libraries(${SHADER_INFO} dl arch usdAi ${PYTHON_LIBRARY} ${Boost_LIBRARIES})

install(TARGETS ${SHADER_INFO}
        DESTINATION bin)