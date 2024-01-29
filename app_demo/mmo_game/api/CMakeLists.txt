find_package(Protobuf REQUIRED)

set(API_PROTO_FILE "./msg.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${API_PROTO_FILE})

add_library(proto-api ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(proto-api PUBLIC protobuf::libprotobuf)
target_include_directories(proto-api PUBLIC ${CMAKE_BINARY_DIR}/app_demo ${Protobuf_INCLUDE_DIRS})
