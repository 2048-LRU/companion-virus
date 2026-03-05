cmake_minimum_required(VERSION 3.0)
project(companion_virus)

add_executable(companion converter.c)

target_link_libraries(companion PUBLIC m)