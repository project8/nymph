/*
 * TestProcessorToolbox.cc
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

#include "ProcessorToolbox.hh"

#include "logger.hh"

#include "catch2/catch_test_macros.hpp"

#ifdef NYMPH_USING_PYTHON
#include <pybind11/embed.h>
namespace py = pybind11;
#endif

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

    SECTION( "Python - no import" )
    {
        LINFO( testlog, "Python Processor Visibility (no python import)");

        REQUIRE_FALSE( tptToolbox.CouldBuild( "hello-world-python" ) );
    }

#ifdef NYMPH_USING_PYTHON
    SECTION( "Python - with import" )
    {
        py::scoped_interpreter guard{};
//        py::module_ sys = py::module_::import("sys");
//        py::print(sys.attr("path"));

        LINFO( testlog, "Python Processor Visibility (with python import)");

        try
        {
            py::module_ nymph = py::module_::import("nymph");
            REQUIRE( tptToolbox.CouldBuild( "hello-world-py" ) );
        }
        catch(const std::exception& e)
        {
            LWARN( testlog, "Unable to import `nymph` ")
        }
        

    }
#endif

}
