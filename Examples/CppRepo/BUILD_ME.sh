#! /bin/bash

# This script builds the CppRepo example of a Nymph-based C++-only package.
# The CMake-based build downloads Nymph using CMake's FetchContent module, 
# rather than including it as a submodule.  Nymph is built using 
# the Scarab/PackageBuilder framework included with Nymph.

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=${Nymph_BUILD_TYPE:=Debug} \
    -DNymph_BUILD_NYMPH_EXE=${Nymph_BUILD_NYMPH_EXE:=FALSE} \
    -DNymph_ENABLE_EXECUTABLES=${Nymph_ENABLE_EXECUTABLES:=TRUE} \
    -DNymph_ENABLE_PYTHON=${Nymph_ENABLE_PYTHON:=FALSE} \
    -DNymph_ENABLE_TESTING=${Nymph_ENABLE_TESTING:=FALSE} \
    -DNymph_SINGLETHREADED=${Nymph_SINGLETHREADED:=FALSE} \
    -DNymph_TAG=${Nymph_TAG:=nymph2_2/develop} \
    ..

make -j${NARG:=} install
