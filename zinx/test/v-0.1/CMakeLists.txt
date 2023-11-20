add_executable(server server.cpp ${ZINX_SRCS})
target_link_libraries(server ${MuduoNet_LIBRARY} pthread)

add_executable(client client.cpp ${ZINX_SRCS})
target_link_libraries(client ${MuduoNet_LIBRARY} pthread)