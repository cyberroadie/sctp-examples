cmake_minimum_required(VERSION 3.2)
project(scpt-examples Go)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CLIENT_SOURCE_FILES client/EchoClient.go)
add_executable(multiEchoTCPClientGo ${CLIENT_SOURCE_FILES})

set(SERVER_SOURCE_FILES server/EchoServer.go)
add_executable(multiEchoTCPServerGo ${SERVER_SOURCE_FILES})
