/*
 * TestReturnBuffer.cc
 *
 *  Created on: Feb 1, 2022
 *      Author: N.S. Oblath
 */

#include "ReturnBuffer.hh"

#include "catch.hpp"

#include "logger.hh"

LOGGER( testlog, "TestReturnBuffer" );

TEST_CASE( "return_buffer_base", "[processor]" )
{
    using namespace Nymph;

    ReturnBufferBase rbb;

    REQUIRE_THROWS_AS( rbb.GetReturn<double>(), Exception );
}

TEST_CASE( "return_buffer", "[processor]" )
{
    using namespace Nymph;

    SECTION( "empty" )
    {
        ReturnBuffer< double > buffer;
        REQUIRE_THROWS_AS( buffer.GetReturn(), Exception );
    }

    SECTION( "full" )
    {
        int intData = 5;
        double doubleData = 100.2;
        std::string stringData( "I'm a string!" );
        ReturnBuffer< int, double, std::string > buffer( intData, doubleData, stringData );

        REQUIRE_NOTHROW( buffer.GetReturn() );
        auto theReturn = buffer.GetReturn();
        REQUIRE( std::get<0>( theReturn ) == 5 );
        REQUIRE( std::get<1>( theReturn ) == Approx(100.2) );
        REQUIRE( std::get<2>( theReturn ) == "I'm a string!" );

        ReturnBufferBase& rbb = buffer;
        auto theRBBReturn = rbb.GetReturn< int, double, std::string >();
        REQUIRE( std::get<0>( theRBBReturn ) == 5 );
        REQUIRE( std::get<1>( theRBBReturn ) == Approx(100.2) );
        REQUIRE( std::get<2>( theRBBReturn ) == "I'm a string!" );

    }
}
