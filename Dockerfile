ARG base_img_repo=python
ARG base_img_tag=3.12.1-slim-bookworm

# This FROM line includes a label so that the dependencies can be built by themselves by using the `--target` argument of `docker build`
FROM ${base_img_repo}:${base_img_tag} AS base

ARG build_type=Release
ARG build_tests_exe=FALSE
ARG narg=2
ARG nymph_tag=dev

ENV NYMPH_BUILD_TYPE=$build_type
ENV NYMPH_BUILD_TESTS_EXE=$build_tests_exe

ENV NYMPH_TAG=$nymph_tag
ENV NYMPH_PREFIX=/usr/local/p8/nymph/$NYMPH_TAG

# Set bash as the default shell
SHELL ["/bin/bash", "-c"]

RUN apt-get update && \
    apt-get clean && \
    apt-get --fix-missing -y install \
        build-essential \
        cmake \
#        gdb \
        git \
        libboost-chrono-dev \
        libboost-filesystem-dev \
        libboost-date-time-dev \
        libboost-system-dev \
        libboost-thread-dev \
        libyaml-cpp-dev \
        rapidjson-dev && \
#        pybind11-dev \
#        wget && \
    rm -rf /var/lib/apt/lists/*

# use pybind11_checkout to specify a tag or branch name to checkout
ARG pybind11_checkout=v3.0.0
ARG pybind11_repo=https://github.com/pybind/pybind11.git
ARG pybind11_name=pybind11
RUN cd /usr/local && \
    git clone ${pybind11_repo} && \
    cd ${pybind11_name} && \
    git checkout ${pybind11_checkout} && \
    mkdir build && \
    cd build && \
    cmake -DPYBIND11_TEST=FALSE .. && \
    make -j${narg} install && \
    cd / && \
    rm -rf /usr/local/${pybind11_name}

RUN mkdir -p $NYMPH_PREFIX &&\
    chmod -R 777 $NYMPH_PREFIX/.. &&\
    cd $NYMPH_PREFIX &&\
    echo "export NYMPH_TAG=${NYMPH_TAG}" >> setup.sh &&\
    echo "export NYMPH_PREFIX=${NYMPH_PREFIX}" >> setup.sh &&\
    echo 'ln -sfT $NYMPH_PREFIX $NYMPH_PREFIX/../current' >> setup.sh &&\
    echo 'export PATH=$NYMPH_PREFIX/bin:$PATH' >> setup.sh &&\
    echo 'export LD_LIBRARY_PATH=$NYMPH_PREFIX/lib:$LD_LIBRARY_PATH' >> setup.sh &&\
    /bin/true
#    echo "source ${COMMON_BUILD_PREFIX}/setup.sh" > setup.sh &&\


########################
########################
FROM base AS dev

RUN apt-get update && \
    apt-get clean && \
    apt-get --fix-missing -y install \
        cmake-curses-gui \
        gdb \
        nano && \
    rm -rf /var/lib/apt/lists/*

########################
########################


########################
FROM base AS build

COPY cmake /tmp_source/cmake
COPY Python /tmp_source/Python
COPY Cpp /tmp_source/Cpp 
#COPY Executables /tmp_source/cpp/Executables
COPY External /tmp_source/External
#COPY Library /tmp_source/cpp/Library
COPY Scarab /tmp_source/Scarab
COPY Testing /tmp_source/Testing
COPY CMakeLists.txt /tmp_source/CMakeLists.txt
COPY NymphConfig.cmake.in /tmp_source/NymphConfig.cmake.in
COPY pyproject.toml /tmp_source/pyproject.toml
#COPY .git /tmp_source/.git

# repeat the cmake command to get the change of install prefix to set correctly (a package_builder known issue)
RUN source $NYMPH_PREFIX/setup.sh &&\
    cd /tmp_source &&\
    mkdir build &&\
    cd build &&\
    cmake -D CMAKE_BUILD_TYPE=$NYMPH_BUILD_TYPE \
          -D CMAKE_INSTALL_PREFIX:PATH=$NYMPH_PREFIX \
          -D Nymph_ENABLE_TESTING:BOOL=$NYMPH_BUILD_TESTS_EXE .. &&\
    cmake -D CMAKE_BUILD_TYPE=$NYMPH_BUILD_TYPE \
          -D CMAKE_INSTALL_PREFIX:PATH=$NYMPH_PREFIX \
          -D Nymph_ENABLE_TESTING:BOOL=$NYMPH_BUILD_TESTS_EXE .. &&\
    make -j${narg} install &&\
    /bin/true

########################
FROM base

COPY --from=build $NYMPH_PREFIX $NYMPH_PREFIX
