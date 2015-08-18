cmake_minimum_required(VERSION 3.2)
project(scpt-examples Go)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(simpleSCTPClientGo client/simpleSCTPClient.go)
add_executable(simpleSCTPServerGo server/simpleSCTPServer.go)
