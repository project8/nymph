# CMakeLists for Nymph
# Author: N. Oblath

# Minimum cmake verison 3.12 required by Scarab
cmake_minimum_required (VERSION 3.12)

#########
# Setup #
#########

cmake_policy( SET CMP0048 NEW ) # version in project()
project( Nymph VERSION 1.5.2 )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Scarab/cmake )
include( PackageBuilder )

# Nymph CMake files
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( Nymph )


#################
# Nymph options #
#################

# require C++11
set( CMAKE_CXX_STANDARD 11 )

# Build the Nymph executable (also requires Nymph_ENABLE_EXECUTABLES)
option( Nymph_BUILD_NYMPH_EXE "Flag to build the Nymph executable; also requires Nymph_ENABLE_EXECUTABLES" ON )

# Build python bindings (requires boost::python)
option( Nymph_ENABLE_PYTHON "Build python bindings (NymphPy)" OFF )

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

set( PUBLIC_EXT_LIBS )
set( PRIVATE_EXT_LIBS )

#######
# Boost
#######

# Boost (1.46 required for filesystem version 3)
list( APPEND BOOST_COMPONENTS date_time filesystem program_options system thread )
find_package( Boost 1.46.0 REQUIRED COMPONENTS ${BOOST_COMPONENTS} )
list( APPEND PUBLIC_EXT_LIBS Boost::boost Boost::date_time Boost::program_options Boost::thread Boost::filesystem )
# make sure dynamic linking is assumed for all boost libraries
set( Boost_USE_STATIC_LIBS OFF )
set( Boost_USE_STATIC_RUNTIME OFF )
set( Boost_ALL_DYN_LINK ON )
add_definitions (-DBOOST_ALL_DYN_LINK)

# Python 

if( Nymph_ENABLE_PYTHON )
    set_option( Scarab_BUILD_PYTHON TRUE )
else( Nymph_ENABLE_PYTHON )
    set_option( Scarab_BUILD_PYTHON FALSE )
endif( Nymph_ENABLE_PYTHON )


#####################
# Prepare for build #
#####################

pbuilder_prepare_project()


##############
# Submodules #
##############

########
# Scarab
########

pbuilder_add_submodule( Scarab Scarab )

# all parts of midge use Scarab, so include it here
pbuilder_use_sm_library( Scarab Scarab )

# this is needed for anything that wants to read or write JSON files
get_directory_property( RAPIDJSON_FILE_BUFFER_SIZE DIRECTORY Scarab/library DEFINITION RAPIDJSON_FILE_BUFFER_SIZE )
set( RAPIDJSON_FILE_BUFFER_SIZE ${RAPIDJSON_FILE_BUFFER_SIZE} PARENT_SCOPE )


###############
# Build Nymph #
###############

add_subdirectory( Library )

if (Nymph_ENABLE_TESTING)
    add_subdirectory( Executables/Validation )
endif (Nymph_ENABLE_TESTING)

if (Nymph_ENABLE_EXECUTABLES)
    add_subdirectory( Executables/Main )
endif (Nymph_ENABLE_EXECUTABLES )


##########
# Config #
##########

configure_file( ${PROJECT_SOURCE_DIR}/NymphConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/NymphConfig.cmake @ONLY )

pbuilder_do_package_config()
