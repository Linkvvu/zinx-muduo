project(MMO)

include_directories(${MuduoNet_INCLUDE_DIR})

file(GLOB_RECURSE MMO_SOURCES "./*.cpp")

add_subdirectory(./api)

add_executable(${PROJECT_NAME} ${MMO_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE zinx-muduo proto-api)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/app_demo)