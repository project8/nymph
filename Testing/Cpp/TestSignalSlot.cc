/*
 * TestSigSlot.cc
 *
 *  Created on: Sept 7, 2019
 *      Author: N.S. Oblath
 */

#include "Signal.hh"
#include "Slot.hh"

#include "TestControllerClasses.hh"

#include "catch.hpp"

#include <future>

TEST_CASE( "signal_slot", "[signal],[slot],[processor]" )
{
    using namespace Nymph;

    // need a controller to exist
    CIQThrowController controller;

    int testValue = 0;

    Signal< int > signal( "signal" );
    Slot< int > slot( "slot", [&](int aValue ){ testValue = aValue; } );

    REQUIRE_FALSE( signal.GetDoBreakpoint() );

    SECTION( "signal_emit" )
    {
        // this section tests all of the paths in Signal::operator()()
        // nothing is listening to the signal, and instead we verify whether exceptions are thrown

        // test successful throw, no unusual circumstances
        REQUIRE_NOTHROW( signal.Emit( 1 ) );

        // test successful throw after breakpoint not set by the signal
        auto asyncReturn = std::async( std::launch::async, [&controller](){
            controller.WaitForBreakOrCanceled();
            std::this_thread::sleep_for( std::chrono::milliseconds(200) );
            controller.Continue();
        } );
        controller.Break();
        REQUIRE_NOTHROW( signal.Emit( 2 ) );

        // test successful throw after breakpoint set by the signal
        signal.SetDoBreakpoint( true );
        asyncReturn = std::async( std::launch::async, [&controller](){
            controller.WaitForBreakOrCanceled();
            std::this_thread::sleep_for( std::chrono::milliseconds(200) );
            controller.Continue();
        } );
        REQUIRE_NOTHROW( signal.Emit( 3 ) );
        signal.SetDoBreakpoint( false );

        // test behavior for canceled controller
        controller.Cancel();
        REQUIRE_THROWS_AS( signal.Emit( 1 ), QuitChain );
        controller.reset_cancel();

        // test behavior for controller canceled during break
        asyncReturn = std::async( std::launch::async, [&controller](){
            controller.WaitForBreakOrCanceled();
            std::this_thread::sleep_for( std::chrono::milliseconds(200) );
            controller.Cancel();
        } );
        controller.Break();
        REQUIRE_THROWS_AS( signal.Emit( 2 ), QuitChain );
        controller.reset_cancel();

        // test behavior for signal starting the break, and controller is canceled during the break
        signal.SetDoBreakpoint( true );
        asyncReturn = std::async( std::launch::async, [&controller](){
            controller.WaitForBreakOrCanceled();
            std::this_thread::sleep_for( std::chrono::milliseconds(200) );
            controller.Cancel();
        } );
        REQUIRE_THROWS_AS( signal.Emit( 3 ), QuitChain );

    }

    SECTION( "connections" )
    {
        // TODO: connect to an incompatible slot and check exception

        REQUIRE_NOTHROW( signal.Connect( &slot ) );

        REQUIRE( signal.Connections().size() == 1 );
        REQUIRE( slot.Connections().size() == 1 );

        signal.Emit( 5 );
        REQUIRE( testValue == 5 );

        signal( 10 );
        REQUIRE( testValue == 10 );

        signal.Disconnect( &slot );

        REQUIRE( signal.Connections().size() == 0 );
        REQUIRE( slot.Connections().size() == 0 );

        // connect and disconnect via slot
        slot.ConnectTo( &signal );

        REQUIRE( signal.Connections().size() == 1 );
        REQUIRE( slot.Connections().size() == 1 );

        slot.Disconnect( &signal );

        REQUIRE( signal.Connections().size() == 0 );
        REQUIRE( slot.Connections().size() == 0 );

        // reconnect for testing SignalBase::DisconnectAll()
        signal.Connect( &slot );

        REQUIRE( signal.Connections().size() == 1 );
        REQUIRE( slot.Connections().size() == 1 );

        signal.DisconnectAll();

        REQUIRE( signal.Connections().size() == 0 );
        REQUIRE( slot.Connections().size() == 0 );

        // reconnect for testing SlotBaseDisconnectAll()
        slot.ConnectTo( &signal );

        REQUIRE( signal.Connections().size() == 1 );
        REQUIRE( slot.Connections().size() == 1 );

        slot.DisconnectAll();

        REQUIRE( signal.Connections().size() == 0 );
        REQUIRE( slot.Connections().size() == 0 );
    }

}
