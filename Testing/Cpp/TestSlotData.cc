/*
 * TestSlotData.cc
 *
 *  Created on: Jan 25, 2022
 *      Author: N.S. Oblath
 */

#include "TestControllerClasses.hh"
#include "TestDataProcessorClasses.hh"

#include "DataFrame.hh"

#include "catch.hpp"

#include "logger.hh"
LOGGER(tsdlog_hh, "testslotdata.hh");

TEST_CASE( "slot_data", "[slot],[data]" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    // need a controller to exist
    CIQThrowController tsdController;


// TODO: test creation of slot with:
//   x proc == owner; no return; multiple inputs
//   x proc == owner; multiple returns; multiple inputs
//   * proc= != owner; multiple returns; multiple inputs
//   x const arguments (supply const with type)

// TODO: test calling of function
//   x multiple inputs; data not present; throws Exception
//   x multiple inputs; multiple output; call is successful
//   x call throws results in thrown Exception

// TODO: test signal emmission
//   x slot with output calls a second slot

    // create the processor and verify the presence of the slots
    Adder tsdAdder;
    REQUIRE( tsdAdder.Slots().count("add-sig") == 1 );
    REQUIRE( tsdAdder.Slots().count("add-no-sig") == 1 );

    // get signal and slot pointers and cast
    SlotData< In<TestData1>, Out<> >* tsdSlotDAddSig = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( tsdAdder.Slots().at("add-sig") );
    SlotData< In<TestData1>, Out<> >* tsdSlotDAddNoSig = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( tsdAdder.Slots().at("add-no-sig") );
    REQUIRE( tsdSlotDAddSig );
    REQUIRE( tsdSlotDAddNoSig );

    SlotData< In<const TestData1>, Out<TestData2> >* tsdSlotDMult = dynamic_cast< SlotData< In<const TestData1>, Out<TestData2> >* >( tsdAdder.Slots().at("multiply") );
    REQUIRE( tsdSlotDMult );

    SlotData< In<const TestData1>, Out<TestData2> >* tsdSlotDIToD = dynamic_cast< SlotData< In<const TestData1>, Out<TestData2> >* >( tsdAdder.Slots().at("i-to-d") );
    REQUIRE( tsdSlotDIToD );

    SlotData< In<const TestData1>, Out<> >* tsdSlotDPrint = dynamic_cast< SlotData< In<const TestData1>, Out<> >* >( tsdAdder.Slots().at("print") );
    REQUIRE( tsdSlotDPrint );

    SlotData< In<TestData1>, Out<> >* tsdSlotDJustThrows = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( tsdAdder.Slots().at("just-throws") );
    REQUIRE( tsdSlotDJustThrows );

    SignalData* tsdSigDAdd = dynamic_cast< SignalData* >( tsdAdder.Signals().at("add") );
    REQUIRE( tsdSigDAdd );

    // configure the processor
    tsdAdder.SetAddValue( 5 );

    // create a data frame
    DataHandle tsdHandle = std::make_shared< DataFrame >();

    // verify that when we call the slot on an empty frame we get an exception
    REQUIRE_THROWS_AS( (*tsdSlotDAddSig)( tsdHandle ), Exception );

    TestData1& tsdTestData1 = tsdHandle->Get< TestData1 >();
    tsdTestData1.SetIValue1( 1 );
    tsdTestData1.SetIValue2( 2 );

    // verify that slots can be called, in this case with an input and no output
    // verify that slots work whether or not signals are automatically called
    // call the add slot functions and verify operation
    REQUIRE_NOTHROW( (*tsdSlotDAddSig)( tsdHandle ) ); // this adds the tsdAdder.fAddValue (i.e. 5) to 1 and 2, respectively
    REQUIRE( tsdTestData1.GetIValue1() == 6 );
    REQUIRE( tsdTestData1.GetIValue2() == 7 );

    REQUIRE_NOTHROW( (*tsdSlotDAddNoSig)( tsdHandle ) ); // this adds the tsdAdder.fAddValue to 6 and 7, respectively
    REQUIRE( tsdTestData1.GetIValue1() == 11 ); // and IValue2 would be 12

    // call the IToD slot function and verify operation
    REQUIRE_NOTHROW( (*tsdSlotDIToD)( tsdHandle ) ); // this adds TestData2 to the frame and sets the integer values in its double variables
    REQUIRE( tsdHandle->Has< TestData2 >() );
    TestData2& tsdTestData2 = tsdHandle->Get< TestData2 >();
    REQUIRE( tsdTestData2.GetDValue1() == Approx(11.) );
    REQUIRE( tsdTestData2.GetDValue2() == Approx(12.) );

    // verify that a slot with a const data object works
    // call the print slot function to verify use with const data
    REQUIRE_NOTHROW( (*tsdSlotDPrint)( tsdHandle ) );

    // verify that a thrown exception appears
    // calls the slot function that just throws Exception; verify it appears here
    REQUIRE_THROWS_AS( (*tsdSlotDJustThrows)( tsdHandle ), Exception );

    // verify that an automatic signal call is made
    // connect signal "add" to slot "i-to-d"
    REQUIRE_NOTHROW( tsdAdder.ConnectSignalToSlot( tsdSigDAdd, tsdSlotDIToD ) );
    // remove TestData2
    tsdHandle->Remove< TestData2 >();  // we'll start fresh without a TestData2
    REQUIRE_FALSE( tsdHandle->Has< TestData2 >() );
    // call the add-sig slot and verify that the i-to-d slot gets called via the signal
    REQUIRE_NOTHROW( (*tsdSlotDAddSig)( tsdHandle ) ); //add slot executes, adding 5 to 11, then triggers the i-to-d slot via the add signal, making TestData2 with fDValue2 = 16
    REQUIRE( tsdHandle->Has< TestData2 >() );
    TestData2& tsdAnotherTestData2 = tsdHandle->Get< TestData2 >();
    REQUIRE( tsdTestData1.GetIValue1() == 16 );
    REQUIRE( tsdAnotherTestData2.GetDValue1() == Approx(16.) );

    // verify that slots owned by something else work
    // remove TestData2
    tsdHandle->Remove< TestData2 >();
    REQUIRE_FALSE( tsdHandle->Has< TestData2 >() );
    // call the mult slot and verify that it worked
    REQUIRE_NOTHROW( (*tsdSlotDMult)( tsdHandle ) ); // for TestData1::fIValue1, multiplies 16 by 2.5 and stores in TestData2::fDValue1
    TestData2& tsdYetAnotherTestData2 = tsdHandle->Get< TestData2 >();
    REQUIRE( tsdYetAnotherTestData2.GetDValue1() == Approx(40.) );
}


