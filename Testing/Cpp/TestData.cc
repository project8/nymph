/*
 * TestData.cc
 *
 *  Created on: Jan 6, 2022
 *      Author: N.S. Oblath
 */

#include "TestDataClasses.hh"

#include "catch.hpp"


TEST_CASE( "data", "[data]" )
{
    using namespace Nymph;

    TestData1 data1;
    REQUIRE( data1.GetIValue1() == 0 );
    REQUIRE( data1.GetIValue2() == 5 );

    data1.SetIValue1( 50 );
    REQUIRE( data1.GetIValue1() == 50 );

    TestData2 data2;
    REQUIRE( data2.GetDValue1() == Approx( 0.0 ) );
    REQUIRE( data2.GetDValue2() == Approx( 10.0 ) );

}
