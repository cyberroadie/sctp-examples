cmake_minimum_required(VERSION 2.8)
project("multi_echo client")

include_directories(SYSTEM "/usr/include")

set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")

set(CMAKE_C_FLAGS "-std=c99 -Wall ")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(echoClient echoClient.c)
