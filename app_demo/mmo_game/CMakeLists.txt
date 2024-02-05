project(MMO)

include_directories(${MuduoNet_INCLUDE_DIR})
file(GLOB_RECURSE MMO_SOURCES "*.cpp")
set(AI_CLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/ai_client.cpp")
list(REMOVE_ITEM MMO_SOURCES ${AI_CLIENT_SOURCES})

add_subdirectory(./api)

add_executable(${PROJECT_NAME} ${MMO_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE zinx-muduo proto-api)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/app_demo)

add_executable("${PROJECT_NAME}_ai_client" ${AI_CLIENT_SOURCES})
target_link_libraries("${PROJECT_NAME}_ai_client" PRIVATE zinx-muduo proto-api)
target_include_directories("${PROJECT_NAME}_ai_client" PRIVATE ${CMAKE_SOURCE_DIR}/app_demo)
