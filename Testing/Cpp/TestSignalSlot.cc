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
    using namespace NymphTesting;

    // need a controller to exist
    CIQThrowController tssController;

    int tssTestValue = 0;

    Signal< int > tssSignal( "tssSignal" );
    Slot< int > tssSlot( "tssSlot", [&](int aValue ){ tssTestValue = aValue; } );

    REQUIRE_FALSE( tssSignal.GetDoBreakpoint() );

    SECTION( "signal_emit" )
    {
        // this section tests all of the paths in Signal::operator()()
        // nothing is listening to the signal, and instead we verify whether exceptions are thrown

        SECTION( "basic emit and after break" )
        {
            // test successful throw, no unusual circumstances
            REQUIRE_NOTHROW( tssSignal.Emit( 1 ) );

            // test successful throw after breakpoint not set by the signal
            auto asyncReturn = std::async( std::launch::async, [&tssController](){
                tssController.WaitForBreakOrCanceled();
                std::this_thread::sleep_for( std::chrono::milliseconds(200) );
                tssController.Continue();
            } );
            tssController.Break();
            REQUIRE_NOTHROW( tssSignal.Emit( 2 ) );
        }

        SECTION( "throw after break by signal" )
        {
            // test successful throw after breakpoint set by the signal
            tssSignal.SetDoBreakpoint( true );
            auto asyncReturn = std::async( std::launch::async, [&tssController](){
                tssController.WaitForBreakOrCanceled();
                std::this_thread::sleep_for( std::chrono::milliseconds(200) );
                tssController.Continue();
            } );
            REQUIRE_NOTHROW( tssSignal.Emit( 3 ) );
        }

        SECTION( "canceled controller" )
        {
            // test behavior for canceled controller
            tssController.Cancel();
            REQUIRE_THROWS_AS( tssSignal.Emit( 1 ), QuitChain );
        }

        SECTION( "cancel during break" )
        {
            // test behavior for controller canceled during break
            auto asyncReturn = std::async( std::launch::async, [&tssController](){
                tssController.WaitForBreakOrCanceled();
                std::this_thread::sleep_for( std::chrono::milliseconds(200) );
                tssController.Cancel();
            } );
            tssController.Break();
            REQUIRE_THROWS_AS( tssSignal.Emit( 2 ), QuitChain );
        }

        SECTION( "cancel during break" )
        {
            // test behavior for signal starting the break, and controller is canceled during the break
            tssSignal.SetDoBreakpoint( true );
            auto asyncReturn = std::async( std::launch::async, [&tssController](){
                tssController.WaitForBreakOrCanceled();
                std::this_thread::sleep_for( std::chrono::milliseconds(200) );
                tssController.Cancel();
            } );
            REQUIRE_THROWS_AS( tssSignal.Emit( 3 ), QuitChain );
        }

        SECTION( "return from break" )
        {
            // test return from break
            tssSignal.SetDoBreakpoint( true );
            auto asyncReturn = std::async( std::launch::async, [&tssSignal, &tssController](){
                tssSignal.Emit( 4 ); // cannot test this with REQUIRE() because tests can currently only be done in one thread
                tssController.WaitForEndOfRun();
            } );
            std::this_thread::sleep_for( std::chrono::milliseconds(200) );
            REQUIRE( tssController.WaitForBreakOrCanceled() );
            REQUIRE( tssController.HasReturn() );
            auto theReturn = tssController.GetReturn< int >();
            REQUIRE( std::get<0>( theReturn ) == 4 );
            std::this_thread::sleep_for( std::chrono::milliseconds(200) );
            tssController.Cancel();
        }
    }

    SECTION( "connections" )
    {
        
        REQUIRE_THROWS_AS( floatSignal.Connect( &slot), ConnectionException );

        REQUIRE_NOTHROW( tssSignal.Connect( &tssSlot ) );

        REQUIRE( tssSignal.Connections().size() == 1 );
        REQUIRE( tssSlot.Connections().size() == 1 );

        tssSignal.Emit( 5 );
        REQUIRE( tssTestValue == 5 );

        tssSignal( 10 );
        REQUIRE( tssTestValue == 10 );

        tssSignal.Disconnect( &tssSlot );

        REQUIRE( tssSignal.Connections().size() == 0 );
        REQUIRE( tssSlot.Connections().size() == 0 );

        // connect and disconnect via slot
        tssSlot.ConnectTo( &tssSignal );

        REQUIRE( tssSignal.Connections().size() == 1 );
        REQUIRE( tssSlot.Connections().size() == 1 );

        tssSlot.Disconnect( &tssSignal );

        REQUIRE( tssSignal.Connections().size() == 0 );
        REQUIRE( tssSlot.Connections().size() == 0 );

        // reconnect for testing SignalBase::DisconnectAll()
        tssSignal.Connect( &tssSlot );

        REQUIRE( tssSignal.Connections().size() == 1 );
        REQUIRE( tssSlot.Connections().size() == 1 );

        tssSignal.DisconnectAll();

        REQUIRE( tssSignal.Connections().size() == 0 );
        REQUIRE( tssSlot.Connections().size() == 0 );

        // reconnect for testing SlotBaseDisconnectAll()
        tssSlot.ConnectTo( &tssSignal );

        REQUIRE( tssSignal.Connections().size() == 1 );
        REQUIRE( tssSlot.Connections().size() == 1 );

        tssSlot.DisconnectAll();

        REQUIRE( tssSignal.Connections().size() == 0 );
        REQUIRE( tssSlot.Connections().size() == 0 );
    }

}
