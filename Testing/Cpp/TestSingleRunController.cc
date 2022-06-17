/*
 * TestSingleRunController.cc
 *
 *  Created on: Jan 17, 2021
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

#include "ProcessorToolbox.hh"
#include "SingleRunController.hh"

#include "logger.hh"
#include "param_codec.hh"

#include "catch.hpp"

LOGGER( testlog, "TestSingleRunController" );

namespace NymphTesting
{
    class SRCRevealer : public Nymph::SingleRunController
    {
        public:
            using Nymph::SingleRunController::SingleRunController;

            using Nymph::SingleRunController::ThreadSource;
            using Nymph::SingleRunController::ThreadSourceGroupT;
            bool AddProcessorToThreadGroup( const std::string& name, ThreadSourceGroupT& group )
            {
                return SingleRunController::AddProcessorToThreadGroup( name, group );
            }
    };
}

TEST_CASE( "single_run_controller" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    ProcessorToolbox tsrcToolbox;
    SRCRevealer tsrcController( tsrcToolbox );

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
            "- pp"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( tsrcToolbox.Configure( config->as_node() ) );
        REQUIRE_NOTHROW( tsrcController.Configure( config->as_node() ) );

        auto ppProc = std::dynamic_pointer_cast< TestPrimaryProc >( tsrcToolbox.GetProcessor( "pp" ) );
        REQUIRE( ppProc );
        ppProc->SetTestSelection( TestPrimaryProc::TestType::SignalNewValue );

        // do the run
        REQUIRE_NOTHROW( tsrcController.Run() );

        // check the results
        REQUIRE( ppProc->GetValue() == ppProc->GetNewValue() );

    }

    SECTION( "RunQueue" )
    {
        LINFO( "RunQueue Tests" );

        std::string config_str(
            "- type: test-primary\n"
            "  name: testprimary-1\n"
            "- type: test-primary\n"
            "  name: testprimary-2\n"
            "- type: test-proc\n"
            "  name: testproc-1\n"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( tsrcToolbox.ConfigureProcessors( config->as_array() ) );

        SRCRevealer::ThreadSourceGroupT group;
        REQUIRE_FALSE( tsrcController.AddProcessorToThreadGroup( "blah", group ) );
        REQUIRE_FALSE( tsrcController.AddProcessorToThreadGroup( "testproc-1", group ) );
        REQUIRE( tsrcController.AddProcessorToThreadGroup( "testprimary-1", group ) );
        REQUIRE( group.size() == 1 );

        REQUIRE_FALSE( tsrcController.PushBackToRunQueue( "testproc-1" ) );
        REQUIRE( tsrcController.PushBackToRunQueue( "testprimary-1" ) );
        REQUIRE( tsrcController.RunQueue().size() == 1 );
        REQUIRE( tsrcController.RunQueue().begin()[0].size() == 1 );
        REQUIRE( tsrcController.PushBackToRunQueue( "testprimary-2" ) );
        REQUIRE( tsrcController.RunQueue().size() == 2 );
        REQUIRE( tsrcController.RunQueue().begin()[0].size() == 1 );
        
        tsrcController.PopBackOfRunQueue();
        REQUIRE( tsrcController.RunQueue().size() == 1 );

        tsrcController.ClearRunQueue();
        REQUIRE( tsrcController.RunQueue().empty() );
        
        REQUIRE( tsrcController.PushBackToRunQueue( {"testprimary-1", "testprimary-2"} ) );
        REQUIRE( tsrcController.RunQueue().size() == 1 );
        REQUIRE( tsrcController.RunQueue()[0].size() == 2 );

    }


}
