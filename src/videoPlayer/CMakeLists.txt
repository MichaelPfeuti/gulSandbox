gul_create_header(shaders/default.frag)
gul_create_header(shaders/default.vert)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(videoPlayer main.cpp
                           default_frag.h
                           default_vert.h)
target_link_libraries(videoPlayer gul)
