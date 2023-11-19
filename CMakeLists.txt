cmake_minimum_required(VERSION 3.5)

project(zinx-muduo)

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall")

# Graceful dir
include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/zinx)