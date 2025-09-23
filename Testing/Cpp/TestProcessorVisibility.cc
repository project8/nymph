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

//    SECTION( "Python" )
//    {
//        LINFO( testlog, "Python Processor Visibility");
//
//    }

}
