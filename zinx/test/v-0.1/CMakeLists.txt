add_executable(s server.cpp ${ZINX_SRCS})
target_link_libraries(s ${MuduoNet_LIBRARY} pthread)

add_executable(c client.cpp ${ZINX_SRCS})
target_link_libraries(c ${MuduoNet_LIBRARY} pthread)