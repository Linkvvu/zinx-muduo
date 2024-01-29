#!/bin/sh

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-${SOURCE_DIR}/build}
BUILD_TYPE=${BUILD_TYPE:-"release"}
CXX=${CXX:-g++}
MuduoNet_LIBRARY_PATH=${MuduoNet_LIBRARY_PATH:-"${MuduoNet_DIR}/lib ${MuduoNet_DIR}/lib64"}
MuduoNet_INCLUDE_DIR=${MuduoNet_INCLUDE_DIR:-"${MuduoNet_DIR}/include"}

mkdir -p ${BUILD_DIR}/${BUILD_TYPE}

cd ${BUILD_DIR}/${BUILD_TYPE}                     \
    && cmake 					                        \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE}                \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON              \
        -DMuduoNet_LIBRARY_PATH=${MuduoNet_LIBRARY_PATH}     \
        -DMuduoNet_INCLUDE_DIR=${MuduoNet_INCLUDE_DIR}  \
        ${SOURCE_DIR}   			                    \
    && make $*
