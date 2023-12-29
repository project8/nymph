/*
 * TestServiceToolbox.cc
 *
 *  Created on: Dec 28, 2023
 *      Author: N.S. Oblath
 */

#include "TestServiceClasses.hh"

#include "ServiceToolbox.hh"

#include "logger.hh"
#include "param_codec.hh"

#include "catch.hpp"

LOGGER( testlog, "TestServiceToolbox" );


TEST_CASE( "service_toolbox" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    //SharedControl::get_instance()->Reset();

    ServiceToolbox tstToolbox;

    SECTION( "AddRemoveServices" )
    {
        LINFO( testlog, "Add/Remove Service tests" );

        std::string svcName1( "testsvc-1" );
        std::string svcName2( "testsvc-2" );

        REQUIRE( tstToolbox.CouldBuild( "test-svc" ) );
        REQUIRE_FALSE( tstToolbox.CouldBuild( svcName1 ) );
        REQUIRE_FALSE( tstToolbox.CouldBuild( svcName2 ) );

        REQUIRE_FALSE( tstToolbox.HasService( svcName1 ) );
        REQUIRE_FALSE( tstToolbox.HasService( svcName2 ) );

        REQUIRE_FALSE( tstToolbox.GetService( svcName1 ) );
        REQUIRE_FALSE( tstToolbox.GetService( svcName2 ) );

        std::string config_str(
            "- type: test-svc\n"
            "  name: testsvc-1\n"
            "  value: 5\n"
            "  string: ok value 1"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( tstToolbox.ConfigureServices( config->as_array() ) );

        auto svc = tstToolbox.GetService( svcName1 );
        REQUIRE( svc );
        REQUIRE( svc->Name() == svcName1 );

        auto testService = std::dynamic_pointer_cast< TestService >( svc );
        REQUIRE( testService );
        REQUIRE( testService->GetValue() == 5 );
        REQUIRE( testService->StringValue() == "ok 1" );

        std::shared_ptr< Service > tp1 = tstToolbox.ReleaseService( svcName1 );
        REQUIRE_FALSE( tstToolbox.GetService( svcName1 ) );

        REQUIRE( tstToolbox.AddService( svcName1, tp1 ) );
        REQUIRE( tstToolbox.GetService( svcName1 ) );

        REQUIRE( tstToolbox.AddService( "test-svc", svcName2 ) );
        REQUIRE( tstToolbox.GetService( svcName2 ) );

        REQUIRE( tstToolbox.HasService( svcName1 ) );
        REQUIRE( tstToolbox.HasService( svcName2 ) );

        REQUIRE( tstToolbox.RemoveService( svcName1 ) );
        REQUIRE_FALSE( tstToolbox.GetService( svcName1 ) );
        REQUIRE( tstToolbox.GetService( svcName2 ) );

        tstToolbox.ClearServices();
        REQUIRE_FALSE( tstToolbox.GetService( svcName2 ) );
    }

}
