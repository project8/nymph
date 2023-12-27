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

    SECTION( "type-based indexing" )
    {
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
        REQUIRE( tdfNewTestData2Ptr == &tdfFrame.Get<TestData2>() ); // we did not make a copy

        // set an object with a unique_ptr
        std::unique_ptr< TestData1 > tdfNewTestData1Ptr = std::make_unique< TestData1 >();
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
        REQUIRE_FALSE( &tdfAnotherTestData1 == &tdfFrame.Get<TestData1>() ); // we made a copy
    }

    SECTION( "string-based indexing" )
    {
        std::string typeName1( scarab::type<TestData1>() );
        std::string typeName2( scarab::type<TestData2>() );

        REQUIRE_FALSE( tdfFrame.Has( typeName1 ) );
        REQUIRE_FALSE( tdfFrame.Has( typeName2 ) );

        // TODO: creating an object with Get() is not yet implemented; this is a workaround
        std::unique_ptr< TestData1 > tdfTestData1PtrWorkaround = std::make_unique< TestData1 >();
        REQUIRE_NOTHROW( tdfFrame.Set( typeName1, std::move(tdfTestData1PtrWorkaround) ) );
        // create a data object  with Get()
        TestData1* tdfData1 = dynamic_cast< TestData1* >( &tdfFrame.Get( typeName1 ) );
        REQUIRE( tdfData1 );
        REQUIRE( tdfFrame.Has( typeName1 ) );
        // ensure TestData1 object was initialized correctly
        REQUIRE( tdfData1->GetIValue1() == 0 );
        REQUIRE( tdfData1->GetIValue2() == 5 );

        // make sure that we haven't made a copy of the data object
        tdfData1->SetIValue1( 50 );
        REQUIRE( tdfData1->GetIValue1() == 50 );
        REQUIRE( dynamic_cast< TestData1* >( &tdfFrame.Get( typeName1 ) )->GetIValue1() == 50 );

        // check const access
        // currently has TestData1; does not have TestData2
        REQUIRE( dynamic_cast< const TestData1* >( &tdfcFrame.Get( typeName1 ) )->GetIValue1() == 50 );
        REQUIRE_THROWS_AS( tdfcFrame.Get( typeName2 ), DataFrameException );

        // remove a data object
        // currently has TestData1; does not have TestData2
        REQUIRE_NOTHROW( tdfFrame.Remove( typeName2 ) );
        REQUIRE_NOTHROW( tdfFrame.Remove( typeName1 ) );
        REQUIRE_FALSE( tdfFrame.Has( typeName1 ) );
        REQUIRE( tdfFrame.Empty() );

        // set an object with a raw pointer
        TestData2* tdfNewTestData2Ptr = new TestData2();
        REQUIRE_NOTHROW( tdfFrame.Set( typeName2, tdfNewTestData2Ptr ) );
        REQUIRE( tdfFrame.Has( typeName2 ) );
        REQUIRE( tdfNewTestData2Ptr == dynamic_cast< TestData2* >( &tdfFrame.Get( typeName2 ) ) ); // we did not make a copy

        // set an object with a unique_ptr
        std::unique_ptr< TestData1 > tdfNewTestData1Ptr = std::make_unique< TestData1 >();
        REQUIRE_NOTHROW( tdfFrame.Set( typeName1, std::move(tdfNewTestData1Ptr) ) );
        REQUIRE_FALSE( tdfNewTestData1Ptr );
        REQUIRE( tdfFrame.Has( typeName1 ) );
        REQUIRE( dynamic_cast< TestData1* >( &tdfFrame.Get( typeName1 ) )->GetIValue1() == 0 ); // check that fIValue1 has the default value; we'll overwrite the data object next, and verify we have the new value

        // TODO: this hasn't been implemented yet
        // verify multiple data types are present (TestData1 and TestData2 should be present at this point)
        //REQUIRE( tdfFrame.Has( typeName1, typeName2 ) );
        //REQUIRE( tdfFrame.Has( typeName2, typeName1 ) );

        // TODO: this hasn't been implemented
        // set an object with a copy of an object
        //TestData1 tdfAnotherTestData1;
        //tdfAnotherTestData1.SetIValue1( 4000 );
        //REQUIRE_NOTHROW( tdfFrame.Set( typeName1, &tdfAnotherTestData1 ) );
        //REQUIRE( tdfAnotherTestData1.GetIValue1() == 4000 );
        //REQUIRE( dynamic_cast< TestData1* >( tdfFrame.Get( typeName1 ) )->GetIValue1() == 4000 );
        //REQUIRE_FALSE( &tdfAnotherTestData1 == dynamic_cast< TestData1* >( tdfFrame.Get( typeName1 ) ) ); // we made a copy
    }
}
