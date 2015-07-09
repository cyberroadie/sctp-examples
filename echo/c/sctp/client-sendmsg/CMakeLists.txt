CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT("echo client")
 
SET(CMAKE_C_FLAGS "-g -std=c99 -pthread -Wall -isystem")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

ADD_EXECUTABLE(echoClient echoClient.c)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  TARGET_LINK_LIBRARIES(echoClient /usr/lib/libsctp.dylib)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
