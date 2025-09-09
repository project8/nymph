# Minimum cmake verison 3.12 required for Scarab
cmake_minimum_required (VERSION 3.12)

# Define the project
cmake_policy( SET CMP0048 NEW ) # version in project()
project( CppRepo VERSION 0.0.0 )

include( FetchContent )
# Get Nymph with FetchContent
# If Nymph is a submodule, then set the SOURCE_DIR option to point to it; also in that case, remove any download specifications
message( STATUS "Getting Nymph" )
option( Nymph_TAG "Version of Nymph to use" nymph2_2/develop )
message( "$$$$$$ tag: ${Nymph_TAG}" )
set( Nymph_DIR nymph_src )
FetchContent_Populate( nymph
    SOURCE_DIR ${Nymph_DIR}
    GIT_REPOSITORY https://github.com/project8/nymph
    GIT_TAG ${Nymph_TAG}
    GIT_SHALLOW TRUE
)

# If nymph were a submodule in ${PROJECT_SOURCE_DIR}/nymph, that would be used in place of ${Nymph_DIR}
list( APPEND CMAKE_MODULE_PATH 
    ${CMAKE_BINARY_DIR}/${Nymph_DIR}/Scarab/cmake
    ${CMAKE_BINARY_DIR}/${Nymph_DIR}/cmake 
)
include( PackageBuilder )
include( Nymph )

#########
# flags #
#########

set( CMAKE_CXX_STANDARD 17 )


################
# dependencies #
################

set( PUBLIC_EXT_LIBS )
set( PRIVATE_EXT_LIBS )




#####################
# prepare for build #
#####################

pbuilder_prepare_project()


##############
# submodules #
##############

# Nymph
pbuilder_add_submodule( Nymph ${CMAKE_BINARY_DIR}/${Nymph_DIR} )

pbuilder_use_sm_library( Nymph Nymph )

#####################
# build the project #
#####################

# add include directories
include_directories( BEFORE
    Library/Processors
    Library/Data
)

# build this project
add_subdirectory( Library )

if( tettigoniidae_ENABLE_EXECUTABLES )
    #add_subdirectory( Executables )
endif()

if( tettigoniidae_ENABLE_TESTING )
    #add_subdirectory( Testing )
endif()


##################
# package config #
##################

#configure_file( ${PROJECT_SOURCE_DIR}/tettigoniidaeConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/tettigoniidaeConfig.cmake @ONLY )

#pbuilder_do_package_config()