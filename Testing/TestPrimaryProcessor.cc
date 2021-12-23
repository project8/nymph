/*
 * TestPrimaryProcessor.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "TestProc.hh"

#include "logger.hh"

#include "catch.hpp"

LOGGER( testlog, "TestPrimaryProcessor" );

REGISTER_PROCESSOR( Nymph, TestPrimaryProc, "test-primary" );


TEST_CASE( "primary_processor", "[primary_processor]" )
{
    using namespace Nymph;

    TestPrimaryProc tester;

    tester.ConnectASlot( "value", tester, "value" );

    tester();

    REQUIRE( tester.GetValue() == tester.GetNewValue() );

    //REQUIRE( tester.ValueSig().GetControl() == &control );
}
