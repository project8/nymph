/*
 * TestControlAccesss.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */


#include "ControlAccess.hh"

#include "catch.hpp"

#include <thread>

TEST_CASE( "control_access", "[control]" )
{
    using namespace Nymph;

    ControlAccess* access = ControlAccess::get_instance();

    SECTION( "no_controller" )
    {
        REQUIRE_FALSE( access->GetControl() );
        REQUIRE_THROWS_AS( access->WaitToContinue(), Exception );
        REQUIRE_THROWS_AS( access->WaitForBreakOrCanceled(), Exception );
        REQUIRE_THROWS_AS( access->WaitForEndOfRun(), Exception );
        REQUIRE_THROWS_AS( access->Continue(), Exception );
        REQUIRE_THROWS_AS( access->Cancel(), Exception );
        REQUIRE_THROWS_AS( access->IsCanceled(), Exception );
        REQUIRE_THROWS_AS( access->Break(), Exception );
        REQUIRE_THROWS_AS( access->IsAtBreak(), Exception );
        REQUIRE_THROWS_AS( access->ChainIsQuitting( "blah" ), Exception );
    }

    Controller control;

    SECTION( "Basics" )
    {
        REQUIRE_FALSE( access->IsCanceled() );
        access->Cancel( 5 );
        REQUIRE( access->IsCanceled() );
    }

    SECTION( "WaitToContinue" )
    {
        REQUIRE( access->WaitToContinue() );
    }

    SECTION( "WaitForBreakOrCanceled" )
    {
        access->Break();
        REQUIRE( access->WaitForBreakOrCanceled() == true );
    }

    SECTION( "BreakContinue" )
    {
        REQUIRE_FALSE( access->IsCanceled() );

        // This thread will do the breaking and continuing
        std::thread thread( [&](){ 
            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            access->Break(); // step 1

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            access->Continue(); // step 2

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            access->Cancel( 0 ); // step 3
        } );
        
        // Waiting for break (step 1)
        REQUIRE( access->WaitForBreakOrCanceled() );
        REQUIRE_FALSE( access->IsCanceled() );

        // Waiting for continue (step 2)
        access->WaitToContinue();
        REQUIRE_FALSE( access->IsCanceled() );

        // Waiting for cancelation (step 3)
        REQUIRE_FALSE( access->WaitForBreakOrCanceled() );
        REQUIRE( access->IsCanceled() );

        thread.join();

    }

    SECTION( "ChainIsQuitting" )
    {
        // As if an error is thrown
        std::exception_ptr exceptPtr = std::make_exception_ptr( Exception() << "Test: ChainIsQuitting, throwing Exception" );
        REQUIRE_NOTHROW( access->ChainIsQuitting( "TestController::ChainIsQuitting::Exception", exceptPtr ) );
    }

    SECTION( "ReturnBuffer" )
    {
        REQUIRE_FALSE( access->HasReturn() );
        REQUIRE_THROWS_AS( access->GetReturn<double>(), Exception ); // have to pick a return argument, so I just picked <double> in this case; it could be anything for this test
        double retval = 5;
        auto retBuf = access->BreakAndReturn(retval);
        // check that we're now at a break point
        REQUIRE( access->IsAtBreak() );
        REQUIRE( access->HasReturn() );
        // we can access the return variable through the buffer
        REQUIRE( std::get<0>( retBuf ) == Approx(5.) );
        // we can change the value of the return variable using the buffer
        std::get<0>( retBuf ) = 10.;
        REQUIRE( retval == Approx(10.) );
    }

}