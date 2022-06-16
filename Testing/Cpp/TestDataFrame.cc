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

    // empty Has<>() should return true
    REQUIRE( frame.Has<>() );

    REQUIRE( frame.DataObjects().empty() );
    REQUIRE( frame.Empty() );
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
    // currently has TestData1; does not have TestData2
    REQUIRE( cFrame.Get< TestData1 >().GetIValue1() == 50 );
    REQUIRE_THROWS_AS( cFrame.Get< TestData2 >(), DataFrameException );

    // remove a data object
    // currently has TestData1; does not have TestData2
    REQUIRE_NOTHROW( frame.Remove< TestData2 >() );
    REQUIRE_NOTHROW( frame.Remove< TestData1 >() );
    REQUIRE_FALSE( frame.Has< TestData1 >() );
    REQUIRE( frame.Empty() );

    // set an object with a raw pointer
    TestData2* newTestData2Ptr = new TestData2();
    REQUIRE_NOTHROW( frame.Set( newTestData2Ptr ) );
    REQUIRE( frame.Has< TestData2 >() );
    REQUIRE( (Data*)newTestData2Ptr == frame.DataObjects()[typeid(TestData2)].get() ); // we did not make a copy

    // set an object with a unique_ptr
    std::unique_ptr< TestData1 > newTestData1Ptr( new TestData1 );
    REQUIRE_NOTHROW( frame.Set( std::move(newTestData1Ptr) ) );
    REQUIRE_FALSE( newTestData1Ptr );
    REQUIRE( frame.Has< TestData1 >() );
    REQUIRE( frame.Get< TestData1 >().GetIValue1() == 0 ); // check that fIValue1 has the default value; we'll overwrite the data object next, and verify we have the new value

    // verify multiple data types are present (TestData1 and TestData2 should be present at this point)
    REQUIRE( frame.Has< TestData1, TestData2 >() );
    REQUIRE( frame.Has< TestData2, TestData1 >() );

    // set an object with a copy of an object
    TestData1 anotherTestData1;
    anotherTestData1.SetIValue1( 4000 );
    REQUIRE_NOTHROW( frame.Set( anotherTestData1 ) );
    REQUIRE( anotherTestData1.GetIValue1() == 4000 );
    REQUIRE( frame.Get< TestData1 >().GetIValue1() == 4000 );
    REQUIRE_FALSE( (Data*)(&anotherTestData1) == frame.DataObjects()[typeid(TestData1)].get() ); // we made a copy

}
