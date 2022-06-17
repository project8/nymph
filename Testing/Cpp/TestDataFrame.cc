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
    using namespace NymphTesting;

    DataFrame tdfFrame;
    const DataFrame& tdfcFrame = tdfFrame;

    // empty Has<>() should return true
    REQUIRE( tdfFrame.Has<>() );

    REQUIRE( tdfFrame.DataObjects().empty() );
    REQUIRE( tdfFrame.Empty() );
    REQUIRE_FALSE( tdfFrame.Has< TestData1 >() );
    REQUIRE_FALSE( tdfFrame.Has< TestData2 >() );

    // create a data object  with Get()
    TestData1& tdfData1 = tdfFrame.Get< TestData1 >();
    REQUIRE( tdfFrame.Has< TestData1 >() );
    // ensure TestData1 object was initialized correctly
    REQUIRE( tdfData1.GetIValue1() == 0 );
    REQUIRE( tdfData1.GetIValue2() == 5 );

    // make sure that we haven't made a copy of the data object
    tdfData1.SetIValue1( 50 );
    REQUIRE( tdfData1.GetIValue1() == 50 );
    REQUIRE( tdfFrame.Get< TestData1 >().GetIValue1() == 50 );

    // check const access
    // currently has TestData1; does not have TestData2
    REQUIRE( tdfcFrame.Get< TestData1 >().GetIValue1() == 50 );
    REQUIRE_THROWS_AS( tdfcFrame.Get< TestData2 >(), DataFrameException );

    // remove a data object
    // currently has TestData1; does not have TestData2
    REQUIRE_NOTHROW( tdfFrame.Remove< TestData2 >() );
    REQUIRE_NOTHROW( tdfFrame.Remove< TestData1 >() );
    REQUIRE_FALSE( tdfFrame.Has< TestData1 >() );
    REQUIRE( tdfFrame.Empty() );

    // set an object with a raw pointer
    TestData2* tdfNewTestData2Ptr = new TestData2();
    REQUIRE_NOTHROW( tdfFrame.Set( tdfNewTestData2Ptr ) );
    REQUIRE( tdfFrame.Has< TestData2 >() );
    REQUIRE( (Data*)tdfNewTestData2Ptr == tdfFrame.DataObjects()[typeid(TestData2)].get() ); // we did not make a copy

    // set an object with a unique_ptr
    std::unique_ptr< TestData1 > tdfNewTestData1Ptr( new TestData1 );
    REQUIRE_NOTHROW( tdfFrame.Set( std::move(tdfNewTestData1Ptr) ) );
    REQUIRE_FALSE( tdfNewTestData1Ptr );
    REQUIRE( tdfFrame.Has< TestData1 >() );
    REQUIRE( tdfFrame.Get< TestData1 >().GetIValue1() == 0 ); // check that fIValue1 has the default value; we'll overwrite the data object next, and verify we have the new value

    // verify multiple data types are present (TestData1 and TestData2 should be present at this point)
    REQUIRE( tdfFrame.Has< TestData1, TestData2 >() );
    REQUIRE( tdfFrame.Has< TestData2, TestData1 >() );

    // set an object with a copy of an object
    TestData1 tdfAnotherTestData1;
    tdfAnotherTestData1.SetIValue1( 4000 );
    REQUIRE_NOTHROW( tdfFrame.Set( tdfAnotherTestData1 ) );
    REQUIRE( tdfAnotherTestData1.GetIValue1() == 4000 );
    REQUIRE( tdfFrame.Get< TestData1 >().GetIValue1() == 4000 );
    REQUIRE_FALSE( (Data*)(&tdfAnotherTestData1) == tdfFrame.DataObjects()[typeid(TestData1)].get() ); // we made a copy

}
