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

    SignalPtr_t signalPtr = std::make_shared< Signal< int > >( "signal" );
    SlotPtr_t slotPtr = std::make_shared< Slot< int > >( "slot", [&](int aValue ){ testValue = aValue; } );

    REQUIRE( signalPtr );
    REQUIRE( slotPtr );

    signalPtr->Connect( slotPtr );

    //REQUIRE( signalPtr->Connections().size() == 1 );
    //REQUIRE( slotPtr->Connections().size() == 1 );

    std::dynamic_pointer_cast< Signal< int > >(signalPtr)->Emit( 5 );
    REQUIRE( testValue == 5 );

    std::dynamic_pointer_cast< Signal< int > >(signalPtr)->operator()( 10 );
    REQUIRE( testValue == 10 );

}
