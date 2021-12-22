FROM project8/p8compute_dependencies:v1.2.0 as nymph_common

ARG build_type=Release
ENV NYMPH_BUILD_TYPE=$build_type
ARG build_tests_exe=FALSE
ENV NYMPH_BUILD_TESTS_EXE=$build_tests_exe

ARG nymph_tag=dev
ENV NYMPH_TAG=$nymph_tag
ENV NYMPH_BUILD_PREFIX=/usr/local/p8/nymph/$NYMPH_TAG

RUN mkdir -p $NYMPH_BUILD_PREFIX &&\
    chmod -R 777 $NYMPH_BUILD_PREFIX/.. &&\
    cd $NYMPH_BUILD_PREFIX &&\
    echo "source ${COMMON_BUILD_PREFIX}/setup.sh" > setup.sh &&\
    echo "export NYMPH_TAG=${NYMPH_TAG}" >> setup.sh &&\
    echo "export NYMPH_BUILD_PREFIX=${NYMPH_BUILD_PREFIX}" >> setup.sh &&\
    echo 'ln -sfT $NYMPH_BUILD_PREFIX $NYMPH_BUILD_PREFIX/../current' >> setup.sh &&\
    echo 'export PATH=$NYMPH_BUILD_PREFIX/bin:$PATH' >> setup.sh &&\
    echo 'export LD_LIBRARY_PATH=$NYMPH_BUILD_PREFIX/lib:$LD_LIBRARY_PATH' >> setup.sh &&\
    /bin/true

########################
FROM nymph_common as nymph_done

COPY cmake /tmp_source/cmake
COPY Executables /tmp_source/Executables
#COPY External /tmp_source/External
COPY Library /tmp_source/Library
COPY Scarab /tmp_source/Scarab
COPY Testing /tmp_source/Testing
COPY CMakeLists.txt /tmp_source/CMakeLists.txt
COPY NymphConfig.cmake.in /tmp_source/NymphConfig.cmake.in
COPY .git /tmp_source/.git

# repeat the cmake command to get the change of install prefix to set correctly (a package_builder known issue)
RUN source $NYMPH_BUILD_PREFIX/setup.sh &&\
    cd /tmp_source &&\
    mkdir build &&\
    cd build &&\
    cmake -D CMAKE_BUILD_TYPE=$NYMPH_BUILD_TYPE \
          -D CMAKE_INSTALL_PREFIX:PATH=$NYMPH_BUILD_PREFIX \
          -D Nymph_ENABLE_TESTING:BOOL=$NYMPH_BUILD_TESTS_EXE .. &&\
    cmake -D CMAKE_BUILD_TYPE=$NYMPH_BUILD_TYPE \
          -D CMAKE_INSTALL_PREFIX:PATH=$NYMPH_BUILD_PREFIX \
          -D Nymph_ENABLE_TESTING:BOOL=$NYMPH_BUILD_TESTS_EXE .. &&\
    make -j3 install &&\
    /bin/true

########################
FROM nymph_common

COPY --from=nymph_done $NYMPH_BUILD_PREFIX $NYMPH_BUILD_PREFIX
