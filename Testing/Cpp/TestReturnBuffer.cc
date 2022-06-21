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

    ReturnBufferBase trbRBB;

    REQUIRE_THROWS_AS( trbRBB.GetReturn<double>(), Exception );
}

TEST_CASE( "return_buffer", "[processor]" )
{
    using namespace Nymph;

    int trbIntData = 5;
    double trbDoubleData = 100.2;
    std::string trbStringData( "I'm a string!" );
    ReturnBuffer< int, double, std::string > trbBuffer( trbIntData, trbDoubleData, trbStringData );

    REQUIRE_NOTHROW( trbBuffer.GetReturn() );
    auto trbTheReturn = trbBuffer.GetReturn();
    REQUIRE( std::get<0>( trbTheReturn ) == 5 );
    REQUIRE( std::get<1>( trbTheReturn ) == Approx(100.2) );
    REQUIRE( std::get<2>( trbTheReturn ) == "I'm a string!" );

    ReturnBufferBase& trbRBB = trbBuffer;
    auto trbTheRBBReturn = trbRBB.GetReturn< int, double, std::string >();
    REQUIRE( std::get<0>( trbTheRBBReturn ) == 5 );
    REQUIRE( std::get<1>( trbTheRBBReturn ) == Approx(100.2) );
    REQUIRE( std::get<2>( trbTheRBBReturn ) == "I'm a string!" );

}
