cmake_minimum_required(VERSION 2.8)

project(gulSandbox)
include(libs/gul/tools/CMake/gulProject.cmake)

gul_setup_project()
gul_create_version()
gul_create_astyle_target()
gul_create_doxygen_target()

add_subdirectory(libs/gul)
add_subdirectory(src)
