cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(msc-project-src)

add_subdirectory(client-sctp_sendv)
add_subdirectory(server-sctp_recvv)

#add_subdirectory(client-sendmsg)
#add_subdirectory(server-recvmsg)
