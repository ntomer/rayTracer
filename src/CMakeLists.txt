cmake_minimum_required(VERSION 3.17)

add_executable(main main.cpp color.hpp vec3.hpp)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})