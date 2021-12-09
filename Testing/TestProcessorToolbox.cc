/*
 * TestProcessorToolbox.cc
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#include "TestProc.hh"

#include "ProcessorToolbox.hh"

#include "logger.hh"
#include "param_codec.hh"

#include "catch.hpp"

LOGGER( testlog, "TestProcessorToolbox" );


TEST_CASE( "processor_toolbox" )
{
    using namespace Nymph;

    ProcessorToolbox toolbox;

    SECTION( "Configuration" )
    {
        REQUIRE_FALSE( toolbox.GetProcessor( "testproc-1" ) );

        std::string config_str(
            "processors:"
            "- type: test-proc"
            "  name: testproc-1"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str );

        toolbox.Configure( config->as_node() );

        REQUIRE( toolbox.GetProcessor( "testproc-1" ) );


    }


}
