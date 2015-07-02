cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(msc-project-src)

add_subdirectory(client)
add_subdirectory(server)

SET(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem ")