# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project(antlr-cpp-test)

add_subdirectory(static)
add_subdirectory(cli)
add_subdirectory(node-addon)