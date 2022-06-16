/*
 * TestSlotData.cc
 *
 *  Created on: Jan 25, 2022
 *      Author: N.S. Oblath
 */

#include "TestControllerClasses.hh"
#include "TestDataProcessorClasses.hh"

#include "catch.hpp"

#include "logger.hh"
LOGGER(tsdlog_hh, "testslotdata.hh");

TEST_CASE( "slot_data", "[slot],[data]" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    // need a controller to exist
    CIQThrowController controller;


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
    Adder adder;
    REQUIRE( adder.Slots().count("add-sig") == 1 );
    REQUIRE( adder.Slots().count("add-no-sig") == 1 );

    // get signal and slot pointers and cast
    SlotData< In<TestData1>, Out<> >* slotdAddSig = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( adder.Slots().at("add-sig") );
    SlotData< In<TestData1>, Out<> >* slotdAddNoSig = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( adder.Slots().at("add-no-sig") );
    REQUIRE( slotdAddSig );
    REQUIRE( slotdAddNoSig );

    SlotData< In<const TestData1>, Out<TestData2> >* sdMult = dynamic_cast< SlotData< In<const TestData1>, Out<TestData2> >* >( adder.Slots().at("multiply") );
    REQUIRE( sdMult );

    SlotData< In<const TestData1>, Out<TestData2> >* sdIToD = dynamic_cast< SlotData< In<const TestData1>, Out<TestData2> >* >( adder.Slots().at("i-to-d") );
    REQUIRE( sdIToD );

    SlotData< In<const TestData1>, Out<> >* sdPrint = dynamic_cast< SlotData< In<const TestData1>, Out<> >* >( adder.Slots().at("print") );
    REQUIRE( sdPrint );

    SlotData< In<TestData1>, Out<> >* sdJustThrows = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( adder.Slots().at("just-throws") );
    REQUIRE( sdJustThrows );

    SignalData* sigdAdd = dynamic_cast< SignalData* >( adder.Signals().at("add") );
    REQUIRE( sigdAdd );

    // configure the processor
    adder.SetAddValue( 5 );

    // create a data frame
    DataHandle handle = std::make_shared< DataFrame >();

    // verify that when we call the slot on an empty frame we get an exception
    REQUIRE_THROWS_AS( (*slotdAddSig)( handle ), Exception );

    TestData1& td1 = handle->Get< TestData1 >();
    td1.SetIValue1( 1 );
    td1.SetIValue2( 2 );

    // verify that slots can be called, in this case with an input and no output
    // verify that slots work whether or not signals are automatically called
    // call the add slot functions and verify operation
    REQUIRE_NOTHROW( (*slotdAddSig)( handle ) ); // this adds the adder.fAddValue (i.e. 5) to 1 and 2, respectively
    REQUIRE( td1.GetIValue1() == 6 );
    REQUIRE( td1.GetIValue2() == 7 );

    REQUIRE_NOTHROW( (*slotdAddNoSig)( handle ) ); // this adds the adder.fAddValue to 6 and 7, respectively
    REQUIRE( td1.GetIValue1() == 11 ); // and IValue2 would be 12

    // call the IToD slot function and verify operation
    REQUIRE_NOTHROW( (*sdIToD)( handle ) ); // this adds TestData2 to the frame and sets the integer values in its double variables
    REQUIRE( handle->Has< TestData2 >() );
    TestData2& td2 = handle->Get< TestData2 >();
    REQUIRE( td2.GetDValue1() == Approx(11.) );
    REQUIRE( td2.GetDValue2() == Approx(12.) );

    // verify that a slot with a const data object works
    // call the print slot function to verify use with const data
    REQUIRE_NOTHROW( (*sdPrint)( handle ) );

    // verify that a thrown exception appears
    // calls the slot function that just throws Exception; verify it appears here
    REQUIRE_THROWS_AS( (*sdJustThrows)( handle ), Exception );

    // verify that an automatic signal call is made
    // connect signal "add" to slot "i-to-d"
    REQUIRE_NOTHROW( adder.ConnectSignalToSlot( sigdAdd, sdIToD ) );
    // remove TestData2
    handle->Remove< TestData2 >();  // we'll start fresh without a TestData2
    REQUIRE_FALSE( handle->Has< TestData2 >() );
    // call the add-sig slot and verify that the i-to-d slot gets called via the signal
    REQUIRE_NOTHROW( (*slotdAddSig)( handle ) ); //add slot executes, adding 5 to 11, then triggers the i-to-d slot via the add signal, making TestData2 with fDValue2 = 16
    REQUIRE( handle->Has< TestData2 >() );
    REQUIRE( td1.GetIValue1() == 16 );
    REQUIRE( td2.GetDValue1() == Approx(16.) );

    // verify that slots owned by something else work
    // remove TestData2
    handle->Remove< TestData2 >();
    REQUIRE_FALSE( handle->Has< TestData2 >() );
    // call the mult slot and verify that it worked
    REQUIRE_NOTHROW( (*sdMult)( handle ) ); // for TestData1::fIValue1, multiplies 16 by 2.5 and stores in TestData2::fDValue1
    REQUIRE( td2.GetDValue1() == Approx(40.) );
}


