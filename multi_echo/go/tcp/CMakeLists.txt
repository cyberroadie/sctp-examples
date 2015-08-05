cmake_minimum_required(VERSION 3.2)
project(scpt-examples Go)

set(CLIENT_SOURCE_FILES client/EchoClient.go)
add_executable(tcpclient ${CLIENT_SOURCE_FILES})

set(SERVER_SOURCE_FILES server/EchoServer.go)
add_executable(tcpserver ${SERVER_SOURCE_FILES})
