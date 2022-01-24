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


namespace Nymph
{
    class ProcTBRevealer : public ProcessorToolbox
    {
        public:
            using ProcessorToolbox::ProcessorToolbox;

            bool ParseSignalSlotName( const std::string& toParse, std::string& nameOfProc, std::string& nameOfSigSlot ) const
            {
                return ProcessorToolbox::ParseSignalSlotName( toParse, nameOfProc, nameOfSigSlot );
            }

            using ProcessorToolbox::ThreadSource;
            using ProcessorToolbox::ThreadSourceGroupT;
            bool AddProcessorToThreadGroup( const std::string& name, ThreadSourceGroupT& group )
            {
                return ProcessorToolbox::AddProcessorToThreadGroup( name, group );
            }
    };
}


TEST_CASE( "processor_toolbox" )
{
    using namespace Nymph;

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
            "processors:\n"
            "- type: test-proc\n"
            "  name: testproc-1"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( toolbox.Configure( config->as_node() ) );

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
        LINFO( testlog, "Connections Tests");

        std::string toParse( "proc:sigslot" );
        std::string parsedProc, parsedSigSlot;

        toolbox.ParseSignalSlotName( toParse, parsedProc, parsedSigSlot );
        REQUIRE( parsedProc == "proc" );
        REQUIRE( parsedSigSlot == "sigslot" );

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

        REQUIRE_NOTHROW( toolbox.Configure( config->as_node() ) );

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

    SECTION( "RunQueue" )
    {
        LINFO( "RunQueue Tests" );

        std::string config_str(
            "processors:\n"
            "- type: test-primary\n"
            "  name: testprimary-1\n"
            "- type: test-primary\n"
            "  name: testprimary-2\n"
            "- type: test-proc\n"
            "  name: testproc-1\n"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( toolbox.Configure( config->as_node() ) );

        ProcTBRevealer::ThreadSourceGroupT group;
        REQUIRE_FALSE( toolbox.AddProcessorToThreadGroup( "blah", group ) );
        REQUIRE_FALSE( toolbox.AddProcessorToThreadGroup( "testproc-1", group ) );
        REQUIRE( toolbox.AddProcessorToThreadGroup( "testprimary-1", group ) );
        REQUIRE( group.size() == 1 );

        REQUIRE_FALSE( toolbox.PushBackToRunQueue( "testproc-1" ) );
        REQUIRE( toolbox.PushBackToRunQueue( "testprimary-1" ) );
        REQUIRE( toolbox.RunQueue().size() == 1 );
        REQUIRE( toolbox.RunQueue().begin()[0].size() == 1 );
        REQUIRE( toolbox.PushBackToRunQueue( "testprimary-2" ) );
        REQUIRE( toolbox.RunQueue().size() == 2 );
        REQUIRE( toolbox.RunQueue().begin()[0].size() == 1 );
        
        toolbox.PopBackOfRunQueue();
        REQUIRE( toolbox.RunQueue().size() == 1 );

        toolbox.ClearRunQueue();
        REQUIRE( toolbox.RunQueue().empty() );
        
        REQUIRE( toolbox.PushBackToRunQueue( {"testprimary-1", "testprimary-2"} ) );
        REQUIRE( toolbox.RunQueue().size() == 1 );
        REQUIRE( toolbox.RunQueue()[0].size() == 2 );

    }

}
