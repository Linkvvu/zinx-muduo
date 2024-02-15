link_libraries(zinx-muduo)
include_directories(${MuduoNet_INCLUDE_DIR})

# echo
set(ECHO_EXAMPLE echo)
add_executable("${ECHO_EXAMPLE}-server" echo/Server.cpp)
add_executable("${ECHO_EXAMPLE}-client" echo/Client.cpp)
target_include_directories("${ECHO_EXAMPLE}-client" PUBLIC ${MuduoNet_INCLUDE_DIR})
