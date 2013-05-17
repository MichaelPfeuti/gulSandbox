find_package(GLFW)
include_directories(${GLFW_INCLUDE_DIR})

find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIR})

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(videoPlayer main.cpp)
target_link_libraries(videoPlayer gul ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARY})
