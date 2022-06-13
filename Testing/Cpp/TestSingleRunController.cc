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

namespace Nymph
{
    class SRCRevealer : public SingleRunController
    {
        public:
            using SingleRunController::SingleRunController;

            using SingleRunController::ThreadSource;
            using SingleRunController::ThreadSourceGroupT;
            bool AddProcessorToThreadGroup( const std::string& name, ThreadSourceGroupT& group )
            {
                return SingleRunController::AddProcessorToThreadGroup( name, group );
            }
    };
}

TEST_CASE( "single_run_controller" )
{
    using namespace Nymph;

    ProcessorToolbox toolbox;
    SRCRevealer controller( toolbox );

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

        REQUIRE_NOTHROW( toolbox.Configure( config->as_node() ) );

        auto ppProc = std::dynamic_pointer_cast< TestPrimaryProc >( toolbox.GetProcessor( "pp" ) );
        REQUIRE( ppProc );
        ppProc->SetTestSelection( TestPrimaryProc::TestType::SignalNewValue );

        // do the run
        REQUIRE_NOTHROW( controller.Run() );

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

        REQUIRE_NOTHROW( toolbox.ConfigureProcessors( config->as_array() ) );

        SRCRevealer::ThreadSourceGroupT group;
        REQUIRE_FALSE( controller.AddProcessorToThreadGroup( "blah", group ) );
        REQUIRE_FALSE( controller.AddProcessorToThreadGroup( "testproc-1", group ) );
        REQUIRE( controller.AddProcessorToThreadGroup( "testprimary-1", group ) );
        REQUIRE( group.size() == 1 );

        REQUIRE_FALSE( controller.PushBackToRunQueue( "testproc-1" ) );
        REQUIRE( controller.PushBackToRunQueue( "testprimary-1" ) );
        REQUIRE( controller.RunQueue().size() == 1 );
        REQUIRE( controller.RunQueue().begin()[0].size() == 1 );
        REQUIRE( controller.PushBackToRunQueue( "testprimary-2" ) );
        REQUIRE( controller.RunQueue().size() == 2 );
        REQUIRE( controller.RunQueue().begin()[0].size() == 1 );
        
        controller.PopBackOfRunQueue();
        REQUIRE( controller.RunQueue().size() == 1 );

        controller.ClearRunQueue();
        REQUIRE( controller.RunQueue().empty() );
        
        REQUIRE( controller.PushBackToRunQueue( {"testprimary-1", "testprimary-2"} ) );
        REQUIRE( controller.RunQueue().size() == 1 );
        REQUIRE( controller.RunQueue()[0].size() == 2 );

    }


}
