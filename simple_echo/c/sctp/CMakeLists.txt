cmake_minimum_required(VERSION 3.2)
project(sctp-examples)

include_directories(SYSTEM "/usr/include")
include_directories(${HEADER_FILES})
set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")

set(CMAKE_C_FLAGS "-std=c99 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(simpleEchoSCTPClientSendmsgC client_sendmsg/simpleEchoSCTPClientSendmsg.c ${HEADER_FILES})
target_link_libraries(simpleEchoSCTPClientSendmsgC myutillib)

add_executable(simpleEchoSCTPClientC client_sendv/simpleEchoSCTPClient.c)
add_executable(simpleEchoSCTPServerC server_recvv/simpleEchoSCTPServer.c)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(simpleEchoSCTPClientSendmsgC /usr/lib/libsctp.dylib)
  target_link_libraries(simpleEchoSCTPServerC /usr/lib/libsctp.dylib)
  target_link_libraries(simpleEchoSCTPClientC /usr/lib/libsctp.dylib)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")