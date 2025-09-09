#! /bin/bash

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=${Nymph_BUILD_TYPE:=Debug} \
    -DNymph_BUILD_NYMPH_EXE=${Nymph_BUILD_NYMPH_EXE:=FALSE} \
    -DNymph_ENABLE_EXECUTABLES=${Nymph_ENABLE_EXECUTABLES:=TRUE} \
    -DNymph_ENABLE_PYTHON=${Nymph_ENABLE_PYTHON:=FALSE} \
    -DNymph_ENABLE_TESTING=${Nymph_ENABLE_TESTING:=FALSE} \
    -DNymph_SINGLETHREADED=${Nymph_SINGLETHREADED:=FALSE} \
    ..

make -j${NARG:=} install
