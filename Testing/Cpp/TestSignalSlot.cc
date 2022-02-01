/*
 * TestSigSlot.cc
 *
 *  Created on: Sept 7, 2019
 *      Author: N.S. Oblath
 */

#include "Signal.hh"
#include "Slot.hh"

#include "catch.hpp"


TEST_CASE( "signal_slot", "[signal],[slot],[processor]" )
{
    using namespace Nymph;

    int testValue = 0;

    Signal< int > signal( "signal" );
    Slot< int > slot( "slot", [&](int aValue ){ testValue = aValue; } );

    signal.Connect( &slot );

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
