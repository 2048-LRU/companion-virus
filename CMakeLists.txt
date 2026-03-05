cmake_minimum_required(VERSION 3.0)
project(companion_virus)

set(CMAKE_C_COMPILER gcc)

add_executable(companion converter.c)

target_link_libraries(companion PRIVATE m)
