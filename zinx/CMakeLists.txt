find_library(MuduoNet_LIBRARY muduo HINTS ${MuduoNet_LIBRARY_PATH} REQUIRED)

if(MuduoNet_INCLUDE_DIR)
    include_directories(${MuduoNet_INCLUDE_DIR})
endif()

find_package(nlohmann_json 3.2.0 REQUIRED)

file(GLOB ZINX_SRCS src/*)

add_library(zinx-muduo ${ZINX_SRCS})
target_link_libraries(zinx-muduo ${MuduoNet_LIBRARY} pthread nlohmann_json::nlohmann_json)
target_compile_definitions(zinx-muduo
    PUBLIC
    -DZINX_CLEAR_ERRMSG # for clear exception message of josn library
)
