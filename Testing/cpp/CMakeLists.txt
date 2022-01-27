# CMakeLists.txt for Nymph/cpp/Testing
# Author: N.S. Oblath
# Created: Aug 23, 2019

##########

include_directories( BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( testing_HEADERS
    TestProc.hh
)

set( testing_SOURCES
    TestControlAccess.cc
    TestData.cc
    TestDataFrame.cc
    TestException.cc
    TestPrimaryProcessor.cc
    TestProcessor.cc
    TestProcessorToolbox.cc
    TestSignalSlot.cc
    UseCatch.cc
)

set( lib_dependencies 
    Nymph
    Catch
)

# Interface library was recommended for Catch:
# https://stackoverflow.com/questions/34896891/catch-lib-unit-testing-and-ctest-cmake-integration/34900012#34900012
add_library( Catch INTERFACE )
target_include_directories( Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Catch )

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
