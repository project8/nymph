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

    SECTION( "AddRemoveProcessors" )
    {
        LINFO( testlog, "Add/Remove Processor tests" );

        std::string procName1( "testproc-1" );
        std::string procName2( "testproc-2" );

        REQUIRE_FALSE( toolbox.GetProcessor( procName1 ) );
        REQUIRE_FALSE( toolbox.GetProcessor( procName2 ) );

        std::string config_str(
            "processors:\n"
            "- type: test-proc\n"
            "  name: testproc-1"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE( toolbox.Configure( config->as_node() ) );

        REQUIRE( toolbox.GetProcessor( procName1 ) );
        REQUIRE( toolbox.GetProcessor( procName1 )->Name() == procName1 );

        std::shared_ptr< Processor > tp1 = toolbox.ReleaseProcessor( procName1 );
        REQUIRE_FALSE( toolbox.GetProcessor( procName1 ) );

        REQUIRE( toolbox.AddProcessor( procName1, tp1 ) );
        REQUIRE( toolbox.GetProcessor( procName1 ) );

        REQUIRE( toolbox.AddProcessor(  "test-proc", procName2 ) );
        REQUIRE( toolbox.GetProcessor( procName2 ) );

        REQUIRE( toolbox.RemoveProcessor( procName1 ) );
        REQUIRE_FALSE( toolbox.GetProcessor( procName1 ) );
        REQUIRE( toolbox.GetProcessor( procName2 ) );

        toolbox.ClearProcessors();
        REQUIRE_FALSE( toolbox.GetProcessor( procName2 ) );
    }

    SECTION( "Connections" )
    {
        LINFO( testlog, "Connections Tests")

        std::string procName1( "testproc-1" );
        std::string procName2( "testproc-2" );

        std::string config_str(
            "processors:\n"
            "- type: test-proc\n"
            "  name: testproc-1\n"
            "connections:\n"
            "- signal: \"testproc-1:value\"\n"
            "  slot: \"testproc-1:second-value\""
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE( toolbox.Configure( config->as_node() ) );

        std::shared_ptr< Processor > tp1 = toolbox.GetProcessor( procName1 );

        REQUIRE( tp1->Signals().at("value")->Connections().size() == 1 );
        REQUIRE( tp1->Slots().at("value")->Connections().size() == 0 );
        REQUIRE( tp1->Signals().at("second-value")->Connections().size() == 0 );
        REQUIRE( tp1->Slots().at("second-value")->Connections().size() == 1 );

        tp1->Signals().at("value")->DisconnectAll();
        REQUIRE( tp1->Signals().at("value")->Connections().empty() );

        toolbox.MakeConnection( "testproc-1:value", "testproc-1:second-value" );
        REQUIRE( tp1->Signals().at("value")->Connections().size() == 1 );
        REQUIRE( tp1->Slots().at("value")->Connections().size() == 0 );
        REQUIRE( tp1->Signals().at("second-value")->Connections().size() == 0 );
        REQUIRE( tp1->Slots().at("second-value")->Connections().size() == 1 );

        tp1->Signals().at("value")->DisconnectAll();
        REQUIRE( tp1->Signals().at("value")->Connections().empty() );

        toolbox.MakeConnection( "testproc-1", "value", "testproc-1", "second-value" );
        REQUIRE( tp1->Signals().at("value")->Connections().size() == 1 );
        REQUIRE( tp1->Slots().at("value")->Connections().size() == 0 );
        REQUIRE( tp1->Signals().at("second-value")->Connections().size() == 0 );
        REQUIRE( tp1->Slots().at("second-value")->Connections().size() == 1 );
    }

}
