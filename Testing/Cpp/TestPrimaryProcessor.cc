/*
 * TestPrimaryProcessor.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "TestControllerClasses.hh"
#include "TestProcessorClasses.hh"

#include "logger.hh"

#include "catch.hpp"

LOGGER( testlog, "TestPrimaryProcessor" );

TEST_CASE( "primary_processor", "[primary_processor]" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    //SharedControl::get_instance()->Reset();

    TestPrimaryProc tester;
    CIQThrowController controller;

    // SignalNewValue test
    tester.SetTestSelection( TestPrimaryProc::TestType::SignalNewValue );
    tester.ConnectASlot( "value", tester, "value" );
    tester();
    REQUIRE( tester.GetValue() == tester.GetNewValue() );

    // WaitTwoSec test
    // -- no test implemented here right now --
    //tester.SetTestSelection( TestPrimaryProc::TestType::WaitTwoSec );

    // ThrowExcept test
    tester.SetTestSelection( TestPrimaryProc::TestType::ThrowExcept );
    REQUIRE_THROWS_AS( tester(), TestPPException );

    
    

    //REQUIRE( tester.ValueSig().GetControl() == &control );
}
