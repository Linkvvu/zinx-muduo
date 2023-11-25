file(GLOB MMO_SOURCES "core/*.cpp" "api/*.cc" "api/*.cpp" "./*cpp")

find_library(MuduoNet_LIBRARY muduo HINTS ${MuduoNet_LIBRARY_PATH} REQUIRED)

if(MuduoNet_INCLUDE_DIR)
    include_directories(${MuduoNet_INCLUDE_DIR})
endif()

find_package(Protobuf CONFIG REQUIRED)

add_executable(mmo ${MMO_SOURCES})

target_link_libraries(mmo zinx-muduo pthread protobuf::libprotobuf)
