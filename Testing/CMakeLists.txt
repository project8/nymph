# CMakeLists for Nymph/Testing
# Author: N.S. Oblath

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

set( testing_HEADERS
    ${CPP_DIR}/TestProc.hh
)

#set (pytesting_HEADERS
#  
#)

#set (pytesting_SOURCES
#
#)

set( testing_SOURCES
    ${CPP_DIR}/TestControlAccess.cc
    ${CPP_DIR}/TestController.cc
    ${CPP_DIR}/TestData.cc
    ${CPP_DIR}/TestDataFrame.cc
    ${CPP_DIR}/TestException.cc
    ${CPP_DIR}/TestPrimaryProcessor.cc
    ${CPP_DIR}/TestProcessor.cc
    ${CPP_DIR}/TestProcessorToolbox.cc
    ${CPP_DIR}/TestSignalSlot.cc
    ${CPP_DIR}/TestSingleRunController.cc
    ${CPP_DIR}/UseCatch.cc
)

set( lib_dependencies 
    Nymph
    Catch
)

# Interface library was recommended for Catch:
# https://stackoverflow.com/questions/34896891/catch-lib-unit-testing-and-ctest-cmake-integration/34900012#34900012

add_library( Catch INTERFACE )
target_include_directories( Catch INTERFACE ${Scarab_LOCATION}/testing/catch )

add_executable( RunTests ${testing_SOURCES} )
target_link_libraries( RunTests ${lib_dependencies} )

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
# NymphPyTesting library 
############################

#pbuilder_library( 
#    TARGET NymphPyTesting
#    SOURCES ${pytesting_SOURCES}
#    PROJECT_LIBRARIES ${lib_dependencies}
#)

#pbuilder_component_install_and_export( 
#    COMPONENT Testing
#    LIBTARGETS NymphPyTesting
#)

#pbuilder_install_headers( ${pytesting_HEADERS} )

############################
# Executables
############################

#set( lib_dependencies
#    NymphPyTesting
#)
