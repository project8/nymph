/*
 * TestSlotData.cc
 *
 *  Created on: Jan 25, 2022
 *      Author: N.S. Oblath
 */

#include "TestControllerClasses.hh"
#include "TestDataProcessorClasses.hh"

#include "catch.hpp"

TEST_CASE( "slot_data", "[slot],[data]" )
{
    using namespace Nymph;

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
    SlotData< In<TestData1>, Out<> >* sdAddSig = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( adder.Slots().at("add-sig") );
    SlotData< In<TestData1>, Out<> >* sdAddNoSig = dynamic_cast< SlotData< In<TestData1>, Out<> >* >( adder.Slots().at("add-no-sig") );
    REQUIRE( sdAddSig );
    REQUIRE( sdAddNoSig );

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
    REQUIRE_THROWS_AS( (*sdAddSig)( handle ), Exception );

    TestData1& td1 = handle->Get< TestData1 >();
    td1.SetIValue1( 1 );
    td1.SetIValue2( 2 );

    // verify that slots can be called, n this case with an input and no output
    // verify that slots work whether or not signals are automatically called
    // call the add slot functions and verify operation
    REQUIRE_NOTHROW( (*sdAddSig)( handle ) );
    REQUIRE( td1.GetIValue1() == 6 );
    REQUIRE( td1.GetIValue2() == 7 );

    REQUIRE_NOTHROW( (*sdAddNoSig)( handle ) );
    REQUIRE( td1.GetIValue1() == 11 );

    // call the IToD slot function and verify operation
    REQUIRE_NOTHROW( (*sdIToD)( handle ) );
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
    handle->Remove< TestData2 >();
    REQUIRE_FALSE( handle->Has< TestData2 >() );
    // call the add-sig slot and verify that the i-to-d slot gets called via the signal
    REQUIRE_NOTHROW( (*sdAddSig)( handle ) );
    REQUIRE( handle->Has< TestData2 >() );
    REQUIRE( td1.GetIValue1() == 16 );
    REQUIRE( td2.GetDValue1() == Approx(16.) );

    // verify that slots owned by something else work
    // remove TestData2
    handle->Remove< TestData2 >();
    REQUIRE_FALSE( handle->Has< TestData2 >() );
    // call the mult slot and verify that it worked
    REQUIRE_NOTHROW( (*sdMult)( handle ) );
    REQUIRE( td2.GetDValue1() == Approx(40.) );
}


