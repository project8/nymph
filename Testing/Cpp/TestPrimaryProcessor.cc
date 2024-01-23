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

    TestPrimaryProc tppTester;
    CIQThrowController tppController;

    // SignalNewValue test
    tppTester.SetTestSelection( TestPrimaryProc::TestType::SignalNewValue );
    tppTester.ConnectASlot( "value", tppTester, "value" );
    tppTester();
    REQUIRE( tppTester.GetValue() == tppTester.GetNewValue() );

    // WaitTwoSec test
    // -- no test implemented here right now --
    //tppTester.SetTestSelection( TestPrimaryProc::TestType::WaitTwoSec );

    // ThrowExcept test
    tppTester.SetTestSelection( TestPrimaryProc::TestType::ThrowExcept );
    REQUIRE_THROWS_AS( tppTester(), TestPPException );

    
    

    //REQUIRE( tppTester.ValueSig().GetControl() == &control );
}
