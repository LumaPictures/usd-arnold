set(SRC
    loader.cpp
    previewSurface.cpp
    primvarReaderFloat2.cpp
    uvTexture.cpp)

add_library(usdArnold SHARED ${SRC})
target_include_directories(usdArnold PRIVATE ${ARNOLD_INCLUDE_DIR})
target_link_libraries(usdArnold PRIVATE ${ARNOLD_LIBRARY})

install(TARGETS usdArnold LIBRARY DESTINATION shaders)
