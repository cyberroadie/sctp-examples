cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(sctp-examples)

add_subdirectory(client-sctp_sendv)
add_subdirectory(server-sctp_recvv)

#add_subdirectory(client-sendmsg)
#add_subdirectory(server-recvmsg)
