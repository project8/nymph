/*
 * TestDataFrame.cc
 *
 *  Created on: Jan 6, 2022
 *      Author: N.S. Oblath
 */

#include "TestDataClasses.hh"

#include "DataFrame.hh"

#include "catch.hpp"


TEST_CASE( "data_frame", "[data]" )
{
    using namespace Nymph;

    DataFrame frame;
    const DataFrame& cFrame = frame;

    REQUIRE( frame.DataObjects().empty() );
    REQUIRE_FALSE( frame.Has< TestData1 >() );
    REQUIRE_FALSE( frame.Has< TestData2 >() );

    // create a data object  with Get()
    TestData1& data1 = frame.Get< TestData1 >();
    REQUIRE( frame.Has< TestData1 >() );
    // ensure TestData1 object was initialized correctly
    REQUIRE( data1.GetIValue1() == 0 );
    REQUIRE( data1.GetIValue2() == 5 );

    // make sure that we haven't made a copy of the data object
    data1.SetIValue1( 50 );
    REQUIRE( data1.GetIValue1() == 50 );
    REQUIRE( frame.Get< TestData1 >().GetIValue1() == 50 );

    // check const access
    REQUIRE( cFrame.Get< TestData1 >().GetIValue1() == 50 );
    REQUIRE_THROWS_AS( cFrame.Get< TestData2 >(), DataFrameException );
}
