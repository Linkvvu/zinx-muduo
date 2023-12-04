project(MMO)

include_directories(${CMAKE_SOURCE_DIR}/app_demo)

if(MuduoNet_INCLUDE_DIR)
    include_directories(${MuduoNet_INCLUDE_DIR})
endif()

file(GLOB MMO_SOURCES "./**/*" "./mmo_server.cpp")

find_package(Protobuf REQUIRED CONFIG)

add_executable(${PROJECT_NAME} ${MMO_SOURCES})
target_link_libraries(${PROJECT_NAME} zinx-muduo protobuf::libprotobuf)