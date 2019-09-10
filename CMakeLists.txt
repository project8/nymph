# CMakeLists for Nymph
# Author: N. Oblath

# Minimum cmake verison 3.1 required for the variable CMAKE_CXX_STANDARD
cmake_minimum_required (VERSION 3.1)

#########
# Setup #
#########

cmake_policy( SET CMP0048 NEW ) # version in project()
project( Nymph VERSION 1.4.7 )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Scarab/cmake )
include( PackageBuilder )

pbuilder_prepare_project()

# Nymph CMake files
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( Nymph )


#################
# Nymph options #
#################

# require C++11
set_option( USE_CPP11 TRUE )

# Build the Nymph executable (also requires Nymph_ENABLE_EXECUTABLES)
option( Nymph_BUILD_NYMPH_EXE "Flag to build the Nymph executable; also requires Nymph_ENABLE_EXECUTABLES" ON )

# Build python bindings (requires boost::python)
option( Nymph_ENABLE_PYTHON "Build python bindings" OFF )

# Add a "Standard" build type
#set (CMAKE_BUILD_TYPE standard)
set (CMAKE_CXX_FLAGS_STANDARD "-O1 -DNDEBUG -DSTANDARD" CACHE STRING "Flags used by the compiler during standard builds.")
set (CMAKE_C_FLAGS_STANDARD "-O1 -DNDEBUG -DSTANDARD" CACHE STRING "Flags used by the compiler during standard builds.")
set (CMAKE_EXE_LINKER_FLAGS_STANDARD "" CACHE STRING "Flags used by the linker during standard builds.")
set (CMAKE_MODULE_LINKER_FLAGS_STANDARD "" CACHE STRING "Flags used by the linker during standard builds.")
set (CMAKE_SHARED_LINKER_FLAGS_STANDARD "" CACHE STRING "Flags used by the linker during standard builds.")
mark_as_advanced(CMAKE_CXX_FLAGS_STANDARD CMAKE_C_FLAGS_STANDARD CMAKE_EXE_LINKER_FLAGS_STANDARD CMAKE_MODULE_LINKER_FLAGS_STANDARD CMAKE_SHARED_LINKER_FLAGS_STANDARD)


# Single-threading option
option (Nymph_SINGLETHREADED "Flag for running in single-threaded mode" OFF)

# Optional compiler flags
set (FLAG_WARNINGS "" CACHE STRING "Extra warning and error related flags")
set (FLAG_WARNING_LEVEL "NORMAL" CACHE STRING "Valid values are NONE(-w), NORMAL(), MORE(-Wall), MOST(-Wall -Wextra)")
set (FLAG_WARN_AS_ERROR FALSE CACHE BOOL "Treat all warnings as errors")
mark_as_advanced(FLAG_WARNINGS)

nymph_process_options()
  

######################
# Nymph dependencies #
######################

########
# Scarab
########

pbuilder_add_submodule( Scarab Scarab/library )

# this is needed for anything that wants to read or write JSON files
get_directory_property( RAPIDJSON_FILE_BUFFER_SIZE DIRECTORY Scarab/library DEFINITION RAPIDJSON_FILE_BUFFER_SIZE )
message( STATUS "is standalone? ${PBUILDER_STANDALONE}" )
if( PBUILDER_STANDALONE )
    set( RAPIDJSON_FILE_BUFFER_SIZE ${RAPIDJSON_FILE_BUFFER_SIZE} )
else( PBUILDER_STANDALONE )
    set( RAPIDJSON_FILE_BUFFER_SIZE ${RAPIDJSON_FILE_BUFFER_SIZE} PARENT_SCOPE )
endif( PBUILDER_STANDALONE )

#######
# Boost
#######

# Boost (1.46 required for filesystem version 3)
list (APPEND Boost_COMPONENTS chrono date_time filesystem program_options system thread)
find_package (Boost 1.46.0 REQUIRED COMPONENTS ${Boost_COMPONENTS})
# make sure dynamic linking is assumed for all boost libraries
add_definitions (-DBOOST_ALL_DYN_LINK)
include_directories (${Boost_INCLUDE_DIRS})
pbuilder_add_ext_libraries (${Boost_LIBRARIES})

##########
# Pybind11
##########

if( Nymph_ENABLE_PYTHON )
    set( PYBIND11_INSTALL ON CACHE BOOL "Pybind11 option to install itself" FORCE )
    add_subdirectory( External/pybind11 )
    include_directories( External/pybind11/include )    
endif( Nymph_ENABLE_PYTHON )

########
# Cereal
########

set( JUST_INSTALL_CEREAL ON CACHE BOOL "Cereal option: only install library" FORCE)
add_subdirectory( External/cereal )
include_directories( External/cereal/include )


###############
# Build Nymph #
###############

include_directories (BEFORE 
    ${PROJECT_SOURCE_DIR}/Library/Utility
    ${PROJECT_SOURCE_DIR}/Library/Data
    ${PROJECT_SOURCE_DIR}/Library/Processor
    ${PROJECT_SOURCE_DIR}/Library/IO
    ${PROJECT_SOURCE_DIR}/Library/Application
)


add_subdirectory( Library )
if (Nymph_ENABLE_EXECUTABLES)
    #add_subdirectory( Executables/Validation )
endif (Nymph_ENABLE_EXECUTABLES)


#########
# Tests #
#########

if( Nymph_ENABLE_TESTING )
    add_subdirectory( Testing )
endif()


####################################
# Variables for the parent project #
####################################

pbuilder_variables_for_parent()
