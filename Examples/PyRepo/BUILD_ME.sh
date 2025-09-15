#! /bin/bash

# This script builds the PyRepo example of a Nymph-based Python-only package.  
# We first have to build the C++ Nymph library, and then we can install the Python Nymph package.
# Finally, we install the PyRepo package.

# This assumes that if the _nymph directory exists, then _nymph has been built and installed
if [[ ! -d _nymph ]]; then
    git clone --recurse-submodules -b ${Nymph_TAG:=nymph2_2/develop} https://github.com/project8/nymph _nymph
    cd _nymph

    mkdir build
    cd build

    cmake -DCMAKE_BUILD_TYPE=${Nymph_BUILD_TYPE:=Debug} \
        -DNymph_BUILD_NYMPH_EXE=${Nymph_BUILD_NYMPH_EXE:=TRUE} \
        -DNymph_ENABLE_EXECUTABLES=${Nymph_ENABLE_EXECUTABLES:=TRUE} \
        -DNymph_ENABLE_PYTHON=${Nymph_ENABLE_PYTHON:=TRUE} \
        -DNymph_ENABLE_TESTING=${Nymph_ENABLE_TESTING:=FALSE} \
        -DNymph_SINGLETHREADED=${Nymph_SINGLETHREADED:=FALSE} \
        ..

    make -j${NARG:=} install
    source bin/add_lib_python_path.sh

    cd ..

    pip install .

    cd ..
else
    source _nymph/build/bin/add_lib_python_path.sh
fi

pip install -e .
