/*
 * TestRunNymph.cc
 *
 *  Created on: Jan 28, 2021
 *      Author: N.S. Oblath
 */

#include "RunNymph.hh"

#include "logger.hh"
#include "param_codec.hh"

#include "catch.hpp"

LOGGER( testlog, "TestSingleRunController" );


TEST_CASE( "run_nymph" )
{
    using namespace Nymph;

    SECTION( "Missing ProcTB")
    {

    }

    SECTION( "Missing Cont" )
    {

    }

    SECTION( "DoRun" )
    {
        std::string config_str(
            "processors:\n"
            "- type: test-primary\n"
            "  name: pp\n"
            "connections:\n"
            "- signal: \"pp:value\"\n"
            "  slot: \"pp:value\"\n"
            "run-queue:\n"
            "- pp\n"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( RunNymph( config->as_node() ) );

    }

}
