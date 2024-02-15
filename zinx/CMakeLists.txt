# find muduo
find_library(MuduoNet_LIBRARY muduo HINTS ${MuduoNet_LIBRARY_PATH} REQUIRED)
include_directories(${MuduoNet_INCLUDE_DIR})

file(GLOB ZINX_SRCS src/*)

add_library(zinx-muduo ${ZINX_SRCS})
target_link_libraries(zinx-muduo ${MuduoNet_LIBRARY} pthread)
target_compile_definitions(zinx-muduo
    PUBLIC
    -DZINX_CLEAR_ERRMSG # for clear exception message of josn library
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)