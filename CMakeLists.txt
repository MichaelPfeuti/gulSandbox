cmake_minimum_required(VERSION 2.8)

project(gulSandbox)
include(libs/gul/tools/CMake/gulProject.cmake)

gul_setup_project()
gul_create_version()
gul_create_astyle_target()
gul_create_doxygen_target()

# include files
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/algorithms")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/algorithms/analyzer")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/algorithms/converter")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/algorithms/filter")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/algorithms/generator")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/base")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/containers")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/image")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/math")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/memory")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/persistance")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/media")
include_directories("${CMAKE_HOME_DIRECTORY}/libs/gul/include/media/manipulator")
include_directories("${CMAKE_BINARY_DIR}/libs/gul/src")

add_subdirectory(libs/gul)
add_subdirectory(src)
