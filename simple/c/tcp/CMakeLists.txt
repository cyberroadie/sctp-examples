cmake_minimum_required(VERSION 3.2)
project(simple-echo)

include_directories(SYSTEM "/usr/include")
include_directories(${HEADER_FILES})

set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")

set(CMAKE_C_FLAGS "-std=c99 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(simpleEchoTCPClientC client/simpleEchoTCPClient.c)
add_executable(simpleEchoTCPServerC server/simpleEchoTCPServer.c ${HEADER_FILES})

target_link_libraries(simpleEchoTCPServerC myutillib)

