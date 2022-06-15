/*
 * TestProcessorToolbox.cc
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

#include "ProcessorToolbox.hh"

#include "logger.hh"
#include "param_codec.hh"

#include "catch.hpp"

LOGGER( testlog, "TestProcessorToolbox" );


namespace NymphTesting
{
    class ProcTBRevealer : public Nymph::ProcessorToolbox
    {
        public:
            using Nymph::ProcessorToolbox::ProcessorToolbox;

            bool ParseSignalSlotName( const std::string& toParse, std::string& nameOfProc, std::string& nameOfSigSlot ) const
            {
                return Nymph::ProcessorToolbox::ParseSignalSlotName( toParse, nameOfProc, nameOfSigSlot );
            }
    };
}


TEST_CASE( "processor_toolbox" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    //SharedControl::get_instance()->Reset();

    ProcTBRevealer toolbox;

    SECTION( "AddRemoveProcessors" )
    {
        LINFO( testlog, "Add/Remove Processor tests" );

        std::string procName1( "testproc-1" );
        std::string procName2( "testproc-2" );

        REQUIRE_FALSE( toolbox.GetProcessor( procName1 ) );
        REQUIRE_FALSE( toolbox.GetProcessor( procName2 ) );

        std::string config_str(
            "- type: test-proc\n"
            "  name: testproc-1\n"
            "  value: 5\n"
            "  string: ok value 1"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( toolbox.ConfigureProcessors( config->as_array() ) );

        auto proc = toolbox.GetProcessor( procName1 );
        REQUIRE( proc );
        REQUIRE( proc->Name() == procName1 );

        auto testProc = std::dynamic_pointer_cast< TestProc >( proc );
        REQUIRE( testProc );
        REQUIRE( testProc->GetValue() == 5 );
        REQUIRE( testProc->StringValue() == "ok 1" );

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
        LINFO( testlog, "Connections Tests");

        std::string toParse( "proc:sigslot" );
        std::string parsedProc, parsedSigSlot;

        toolbox.ParseSignalSlotName( toParse, parsedProc, parsedSigSlot );
        REQUIRE( parsedProc == "proc" );
        REQUIRE( parsedSigSlot == "sigslot" );

        std::string procName1( "testproc-1" );
        std::string procName2( "testproc-2" );

        std::string p_config_str(
            "- type: test-proc\n"
            "  name: testproc-1"
        );

        std::string c_config_str(
            "- signal: \"testproc-1:value\"\n"
            "  slot: \"testproc-1:second-value\""
        );

        scarab::param_translator translator;
        auto p_config = translator.read_string( p_config_str, "yaml" );
        auto c_config = translator.read_string( c_config_str, "yaml" );

        REQUIRE_NOTHROW( toolbox.ConfigureProcessors( p_config->as_array() ) );
        REQUIRE_NOTHROW( toolbox.ConfigureConnections( c_config->as_array() ) );

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

    SECTION( "FullConfig" )
    {
        std::string testprimaryName( "testprimary" );
        std::string testprocName( "testproc" );

        std::string config_str(
            "processors:\n"
            "- type: test-primary\n"
            "  name: testprimary\n"
            "- type: test-proc\n"
            "  name: testproc\n"
            "connections:\n"
            "- signal: \"testprimary:value\"\n"
            "  slot: \"testproc:value\"\n"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( toolbox.Configure( config->as_node() ) );

        // processors
        REQUIRE( toolbox.GetProcessor( testprimaryName ) );
        REQUIRE( toolbox.GetProcessor( testprimaryName )->Name() == testprimaryName );

        REQUIRE( toolbox.GetProcessor( testprocName ) );
        REQUIRE( toolbox.GetProcessor( testprocName )->Name() == testprocName );

        // connections
        std::shared_ptr< Processor > testprimary = toolbox.GetProcessor( testprimaryName );
        REQUIRE( testprimary->Signals().at("value")->Connections().size() == 1 );

        std::shared_ptr< Processor > testproc = toolbox.GetProcessor( testprocName );
        REQUIRE( testproc->Signals().at("value")->Connections().size() == 0 );
        REQUIRE( testproc->Slots().at("value")->Connections().size() == 1 );
    }

}
