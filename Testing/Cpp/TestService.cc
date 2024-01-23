/*
 * TestService.cc
 *
 *  Created on: Sept 12, 2019
 *      Author: N.S. Oblath
 */

#include "TestControllerClasses.hh"
#include "TestServiceClasses.hh"

#include "ConfigException.hh"

#include "logger.hh"

#include "catch.hpp"

LOGGER( testlog, "TestService" );

TEST_CASE( "service", "[service]" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    TestService tpService;

    SECTION( "Configuration" )
    {
        REQUIRE( tpService.GetValue() == 0 );

        // setup the config parameter
        scarab::param_node config;
        int configValue = 5;
        config.add( "value", configValue );

        // perform configuration with no value for "string"
        REQUIRE_NOTHROW( tpService.Configure( config ) );
        // check that the value was set correctly
        REQUIRE( tpService.GetValue() == configValue );

        // peform configuration with an illegal value for "string"
        config.add( "string", "illegal value" );
        REQUIRE_THROWS_AS( tpService.Configure( config ), ConfigException );
        try
        {
            tpService.Configure( config );
        }
        catch( const ConfigException& e )
        {
            PrintException(e);
        }
        
        // perform configuration with a legal value for "string"
        config["string"]().set( "ok value 1" );
        REQUIRE_NOTHROW( tpService.Configure( config ) );
        REQUIRE( tpService.StringValue() == "ok 1" );

    }
}
