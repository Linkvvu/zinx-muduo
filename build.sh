#!/bin/sh

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-${SOURCE_DIR}/build}
BUILD_TYPE=${BUILD_TYPE:-"Debug"}
CXX=${CXX:-g++}
MuduoNet_LIBRARY_PATH=${MuduoNet_LIBRARY_PATH:-"/usr/local/lib"}
MuduoNet_INCLUDE_DIR=${MuduoNet_INCLUDE_DIR:-"/usr/local/include"}

mkdir -p ${BUILD_DIR}/${BUILD_TYPE}-cpp11

cd ${BUILD_DIR}/${BUILD_TYPE}-cpp11                     \
    && cmake 					                        \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE}                \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON              \
        -DMuduoNet_LIBRARY_PATH=${MuduoNet_LIBRARY_PATH}     \
        -DMuduoNet_INCLUDE_DIR=${MuduoNet_INCLUDE_DIR}  \
        ${SOURCE_DIR}   			                    \
    && make $*
