find_library(MuduoNet_LIBRARY muduo HINTS ${MuduoNet_LIBRARY_PATH} REQUIRED)

if(MuduoNet_INCLUDE_DIR)
    include_directories(${MuduoNet_INCLUDE_DIR})
endif()

file(GLOB ZINX_SRCS src/*)

# add_executable(${PROJECT_NAME}_server ${ZINX_SRCS})
# target_link_libraries(${PROJECT_NAME}_server ${MuduoNet_LIBRARY} pthread)