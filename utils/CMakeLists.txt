set(SHADER_INFO usdAiShaderInfo)

add_executable(${SHADER_INFO} usdAiShaderInfo.cpp)
set_target_properties(${SHADER_INFO} PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)
target_include_directories(${SHADER_INFO} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${SHADER_INFO} PRIVATE ${USD_INCLUDE_DIR})
target_include_directories(${SHADER_INFO} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}../lib)
target_link_libraries(${SHADER_INFO} dl tf gf sdf usd usdGeom usdAi)

install(TARGETS ${SHADER_INFO}
        DESTINATION bin)