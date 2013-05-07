find_package(GLFW)
include_directories(${GLFW_INCLUDE_DIR})

add_executable(videoPlayer main.cpp)
target_link_libraries(videoPlayer gul ${GLFW_LIBRARY})
