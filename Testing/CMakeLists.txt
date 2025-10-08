# CMakeLists for Nymph/Testing
# Author: N.S. Oblath

include( FetchContent )

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)

FetchContent_MakeAvailable( Catch2 )
list( APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras )

pbuilder_component_install_and_export( 
    COMPONENT Catch2
    LIBTARGETS Catch2
    NAMESPACE Catch2::
)

########################################################################

set( PY_DIR Python )
set( CPP_DIR Cpp )

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}/${PY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPP_DIR}
)

#set (pytesting_CONFIGFILES
#    ${PY_DIR}/ConfigFiles/TestConfigurator.json
#)

#pbuilder_install_config (${pytesting_CONFIGFILES})

set( testing_lib_HEADERS
    ${CPP_DIR}/TestControllerClasses.hh
    ${CPP_DIR}/TestDataClasses.hh
    ${CPP_DIR}/TestDataProcessorClasses.hh
    ${CPP_DIR}/TestProcessorClasses.hh
    ${CPP_DIR}/TestServiceClasses.hh
)

set( testing_lib_SOURCES
    ${CPP_DIR}/TestProcessorClasses.cc
    ${CPP_DIR}/TestServiceClasses.cc
)

set( testing_lib_LIB_DEPENDENCIES 
    Nymph
)

pbuilder_library( 
    TARGET NymphTesting
    SOURCES ${testing_lib_SOURCES}
    PUBLIC_EXTERNAL_LIBRARIES ${testing_lib_LIB_DEPENDENCIES}
)

pbuilder_component_install_and_export( 
    COMPONENT Library
    LIBTARGETS NymphTesting
)

pbuilder_install_headers( ${testing_lib_HEADERS} )

set( testing_SOURCES
    ${CPP_DIR}/TestConfigException.cc
    ${CPP_DIR}/TestControlAccess.cc
    ${CPP_DIR}/TestController.cc
    ${CPP_DIR}/TestData.cc
    ${CPP_DIR}/TestDataFrame.cc
    ${CPP_DIR}/TestException.cc
    ${CPP_DIR}/TestPrimaryProcessor.cc
    ${CPP_DIR}/TestProcessor.cc
    ${CPP_DIR}/TestProcessorToolbox.cc
    ${CPP_DIR}/TestProcessorVisibility.cc
    ${CPP_DIR}/TestReturnBuffer.cc
    ${CPP_DIR}/TestRunNymph.cc
    ${CPP_DIR}/TestService.cc
    ${CPP_DIR}/TestServiceToolbox.cc
    ${CPP_DIR}/TestSignalSlot.cc
    ${CPP_DIR}/TestSingleRunController.cc
    ${CPP_DIR}/TestSlotData.cc
    ${CPP_DIR}/UseCatch.cc
)

set( testing_LIB_DEPENDENCIES 
    Nymph
    NymphTesting
)

#if( Nymph_ENABLE_PYTHON )
#    set( testing_LIB_DEPENDENCIES
#        ${testing_LIB_DEPENDENCIES}
#        nymph_bindings
#    )
#endif( Nymph_ENABLE_PYTHON )

# Interface library was recommended for Catch:
# https://stackoverflow.com/questions/34896891/catch-lib-unit-testing-and-ctest-cmake-integration/34900012#34900012
#add_library( Catch INTERFACE )
#target_include_directories( Catch INTERFACE ${Scarab_LOCATION}/testing/catch )

pbuilder_executable(
    EXECUTABLE RunTests
    SOURCES ${testing_SOURCES}
    PROJECT_LIBRARIES ${testing_LIB_DEPENDENCIES}
    PUBLIC_EXTERNAL_LIBRARIES Catch2::Catch2 pybind11::module pybind11::embed
)
set( programs "RunTests" )

pbuilder_component_install_and_export(
    COMPONENT Executables
    EXETARGETS ${programs}
)

# So far it seems sufficient to use Catch2's ability to run tests, and not CTest.
# Here are two ways it could potentially be integrated with CTest, if we change our minds.

# Normal CMake Integration
#enable_testing()
#add_test(NAME test_test COMMAND run_tests )

# Automated CMake Integration
#include(CTest)
#include(Catch)
#catch_discover_tests(foo)

############################
# NymphTesting Python library 
############################

if( Nymph_ENABLE_PYTHON )
    add_subdirectory( Python/Bindings )
endif()
