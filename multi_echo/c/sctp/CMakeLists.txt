cmake_minimum_required(VERSION 3.2)
project(sctp-examples)

set(CMAKE_C_FLAGS "-std=c99 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(multiEchoSCTPClientC client/multiEchoSCTPClient.c)
add_executable(multiEchoSCTPServerC server/multiEchoSCTPServer.c)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(multiEchoSCTPServerC /usr/lib/libsctp.dylib)
  target_link_libraries(multiEchoSCTPClientC /usr/lib/libsctp.dylib)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
