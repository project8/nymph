/*
 * TestData.cc
 *
 *  Created on: Jan 6, 2022
 *      Author: N.S. Oblath
 */

#include "TestDataClasses.hh"

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"


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
    REQUIRE( tdData2.GetDValue1() == Catch::Approx( 0.0 ) );
    REQUIRE( tdData2.GetDValue2() == Catch::Approx( 10.0 ) );

}
