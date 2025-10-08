/*
 * TestProcessorToolbox.cc
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

#include "ProcessorToolbox.hh"

#include "logger.hh"

#include <pybind11/pybind11.h>

#include "catch2/catch_test_macros.hpp"

namespace py = pybind11;

LOGGER( testlog, "TestProcessorVisibility" );

TEST_CASE( "processor_visibility" )
{
    using namespace Nymph;

    ProcessorToolbox tptToolbox;

    SECTION( "Cpp" )
    {
        LINFO( testlog, "C++ Processor Visibility" );

        REQUIRE( tptToolbox.CouldBuild( "hello-world-cpp" ) );
        REQUIRE( tptToolbox.CouldBuild( "test-proc" ) );
        REQUIRE( tptToolbox.CouldBuild( "test-primary" ) );
        REQUIRE_FALSE( tptToolbox.CouldBuild( "does not exist" ) );

    }

//    SECTION( "Python - no import" )
//    {
//        LINFO( testlog, "Python Processor Visibility");
//
//#ifdef NYMPH_USING_PYTHON
//        REQUIRE_FALSE( tptToolbox.CouldBuild( "hello-world-python" ) );
//#else
//        REQUIRE_FALSE( tptToolbox.CouldBuild( "hello-world-python" ) );
//#endif
//    }
//
//    SECTION( "Python - with import" )
//    {
//        LINFO( testlog, "Python Processor Visibility");
//
//#ifdef NYMPH_USING_PYTHON
//        py::module_ nymph = py::module_::import("nymph");
//
//        REQUIRE( tptToolbox.CouldBuild( "hello-world-python" ) );
//#else
//        REQUIRE_FALSE( tptToolbox.CouldBuild( "hello-world-python" ) );
//#endif
//    }

}
