find_library(MuduoNet_LIBRARY muduo HINTS ${MuduoNet_LIBRARY_PATH} REQUIRED)

if(MuduoNet_INCLUDE_DIR)
    include_directories(${MuduoNet_INCLUDE_DIR})
endif()

find_package(nlohmann_json 3.2.0 REQUIRED)

file(GLOB ZINX_SRCS src/*)

add_subdirectory(test/v-0.1)
