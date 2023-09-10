/*
 * TestData.cc
 *
 *  Created on: Jan 6, 2022
 *      Author: N.S. Oblath
 */

#include "TestDataClasses.hh"

#include "catch.hpp"

#include "logger.hh"
#include "typename.hh"

LOGGER( testlog, "TestData" );

TEST_CASE( "data", "[data]" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    TestData1 tdData1;
    REQUIRE( tdData1.GetIValue1() == 0 );
    REQUIRE( tdData1.GetIValue2() == 5 );

    tdData1.SetIValue1( 50 );
    REQUIRE( tdData1.GetIValue1() == 50 );

    TestData2 tdData2;
    REQUIRE( tdData2.GetDValue1() == Approx( 0.0 ) );
    REQUIRE( tdData2.GetDValue2() == Approx( 10.0 ) );

    LWARN( testlog, "Data1: " << scarab::type<TestData1>() );
    LWARN( testlog, "Data1: " << scarab::type<TestData2>() );
}
