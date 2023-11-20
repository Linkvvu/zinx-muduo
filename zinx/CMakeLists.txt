find_library(MuduoNet_LIBRARY muduo HINTS ${MuduoNet_LIBRARY_PATH} REQUIRED)

if(MuduoNet_INCLUDE_DIR)
    include_directories(${MuduoNet_INCLUDE_DIR})
endif()

file(GLOB ZINX_SRCS src/*)

add_subdirectory(test/v-0.1)
