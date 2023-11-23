add_executable(server server.cpp ${ZINX_SRCS})
target_link_libraries(server ${MuduoNet_LIBRARY} pthread nlohmann_json::nlohmann_json)
target_compile_definitions(server
    PRIVATE
    -DZINX_CLEAR_ERRMSG
)

add_executable(client client.cpp ${ZINX_SRCS})
target_link_libraries(client ${MuduoNet_LIBRARY} pthread nlohmann_json::nlohmann_json)