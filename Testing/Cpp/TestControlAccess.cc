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

    ControlAccess* tcaControlAccess = ControlAccess::get_instance();

    SECTION( "no_controller" )
    {
        REQUIRE_FALSE( tcaControlAccess->GetControl() );
        REQUIRE_THROWS_AS( tcaControlAccess->WaitToContinue(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->WaitForBreakOrCanceled(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->WaitForEndOfRun(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->Continue(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->Cancel(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->IsCanceled(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->Break(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->IsAtBreak(), Exception );
        REQUIRE_THROWS_AS( tcaControlAccess->ChainIsQuitting( "blah" ), Exception );
    }

    Controller tcaControl;

    SECTION( "Basics" )
    {
        REQUIRE_FALSE( tcaControlAccess->IsCanceled() );
        tcaControlAccess->Cancel( 5 );
        REQUIRE( tcaControlAccess->IsCanceled() );
    }

    SECTION( "WaitToContinue" )
    {
        REQUIRE( tcaControlAccess->WaitToContinue() );
    }

    SECTION( "WaitForBreakOrCanceled" )
    {
        tcaControlAccess->Break();
        REQUIRE( tcaControlAccess->WaitForBreakOrCanceled() == true );
    }

    SECTION( "BreakContinue" )
    {
        REQUIRE_FALSE( tcaControlAccess->IsCanceled() );

        // This thread will do the breaking and continuing
        std::thread thread( [&](){ 
            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            tcaControlAccess->Break(); // step 1

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            tcaControlAccess->Continue(); // step 2

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            tcaControlAccess->Cancel( 0 ); // step 3
        } );
        
        // Waiting for break (step 1)
        REQUIRE( tcaControlAccess->WaitForBreakOrCanceled() );
        REQUIRE_FALSE( tcaControlAccess->IsCanceled() );

        // Waiting for continue (step 2)
        tcaControlAccess->WaitToContinue();
        REQUIRE_FALSE( tcaControlAccess->IsCanceled() );

        // Waiting for cancelation (step 3)
        REQUIRE_FALSE( tcaControlAccess->WaitForBreakOrCanceled() );
        REQUIRE( tcaControlAccess->IsCanceled() );

        thread.join();

    }

    SECTION( "ChainIsQuitting" )
    {
        // As if an error is thrown
        std::exception_ptr exceptPtr = std::make_exception_ptr( Exception() << "Test: ChainIsQuitting, throwing Exception" );
        REQUIRE_NOTHROW( tcaControlAccess->ChainIsQuitting( "TestController::ChainIsQuitting::Exception", exceptPtr ) );
    }

    SECTION( "ReturnBuffer" )
    {
        REQUIRE_FALSE( tcaControlAccess->HasReturn() );
        REQUIRE_THROWS_AS( tcaControlAccess->GetReturn<double>(), Exception ); // have to pick a return argument, so I just picked <double> in this case; it could be anything for this test
        double retval = 5;
        auto retBuf = tcaControlAccess->BreakAndReturn(retval);
        // check that we're now at a break point
        REQUIRE( tcaControlAccess->IsAtBreak() );
        REQUIRE( tcaControlAccess->HasReturn() );
        // we can tcaControlAccess the return variable through the buffer
        REQUIRE( std::get<0>( retBuf ) == Approx(5.) );
        // we can change the value of the return variable using the buffer
        std::get<0>( retBuf ) = 10.;
        REQUIRE( retval == Approx(10.) );
    }

}