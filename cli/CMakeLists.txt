
set(CMAKE_CXX_STANDARD 17)
add_executable(cli main.cpp)
target_link_libraries(cli antlr-cpp-static)