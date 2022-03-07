# CMakeLists.txt file for Nymph/Python
# Author: A. Ziegler
# Created on: Feb 4, 2022

# Minimum cmake verison 3.12 required by Scarab
cmake_minimum_required (VERSION 3.12)

#########
# Setup #
#########
cmake_policy( SET CMP0048 NEW ) # version in project()
project( NymphPy VERSION 2.0.0 )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../Src/Scarab/cmake )
include( PackageBuilder )

###############
# Build NymphPy #
###############

message( "Building Python bindings")

set( dir ${CMAKE_CURRENT_SOURCE_DIR} )

# Python binding headers
set( PYBINDING_HEADERFILES

)

# Python bindings
set( PYBINDING_SOURCEFILES

)

set( PROJECT_LIBRARIES Scarab )

pbuilder_add_pybind11_module( scarab ${PROJECT_LIBRARIES} )

pbuilder_install_headers( ${PYBINDING_HEADERFILES} )

message( "Done with Python bindings" )