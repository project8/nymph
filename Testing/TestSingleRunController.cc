/*
 * TestSingleRunController.cc
 *
 *  Created on: Jan 17, 2021
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

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

            void StartMultiThreadedRun()
            {
                return SingleRunController::StartMultiThreadedRun();
            }

    };
}


TEST_CASE( "processor_toolbox" )
{
    using namespace Nymph;

    SRCRevealer controller;

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
        REQUIRE_NOTHROW( toolbox.Run() );

        // check the results
        REQUIRE( ppProc->GetValue() == ppProc->GetNewValue() );

    }

}
