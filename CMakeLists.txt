cmake_minimum_required(VERSION 3.2)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/)

project(sctp-examples)

add_subdirectory(multi_echo/c/tcp)
add_subdirectory(multi_echo/go/tcp)
add_subdirectory(multi_echo/c/sctp)
