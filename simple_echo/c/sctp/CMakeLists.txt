cmake_minimum_required(VERSION 3.2)
project(sctp-examples)

include_directories(SYSTEM "/usr/include")
include_directories(${HEADER_FILES})
set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")

set(CMAKE_C_FLAGS "-std=c99 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# As described in https://tools.ietf.org/html/rfc6458#section-3 (One to many style)
add_executable(simpleEchoSCTPClient_sendmsg client_sendmsg/simpleEchoSCTPClient_sendmsg.c ${HEADER_FILES})
target_link_libraries(simpleEchoSCTPClient_sendmsg myutillib)
add_executable(simpleEchoSCTPServer_recvmsg server_recvmsg/simpleEchoSCTPServer_recvmsg.c ${HEADER_FILES})
target_link_libraries(simpleEchoSCTPServer_recvmsg myutillib)

# As described in https://tools.ietf.org/html/rfc6458#section-9.12
add_executable(simpleEchoSCTPClient_sctp_sendv client_sendv/simpleEchoSCTPClient_sctp_sendv.c)
# As described in https://tools.ietf.org/html/rfc6458#section-9.13
add_executable(simpleEchoSCTPServer_sctp_revv server_recvv/simpleEchoSCTPServer_sctp_revv.c)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(simpleEchoSCTPClient_sendmsg /usr/lib/libsctp.dylib)
  target_link_libraries(simpleEchoSCTPServer_recvmsg /usr/lib/libsctp.dylib)
  target_link_libraries(simpleEchoSCTPClient_sctp_sendv /usr/lib/libsctp.dylib)
  target_link_libraries(simpleEchoSCTPServer_sctp_revv /usr/lib/libsctp.dylib)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")