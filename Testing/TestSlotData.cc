/*
 * TestSlotData.cc
 *
 *  Created on: Jan 25, 2022
 *      Author: N.S. Oblath
 */

#include "TestDataProcessorClasses.hh"

#include "catch.hpp"

TEST_CASE( "signal_slot", "[slot],[data]" )
{
    using namespace Nymph;

// TODO: test creation of slot with:
//   * proc == owner; no return; multiple inputs
//   * proc == owner; multiple returns; multiple inputs
//   * proc != owner; no return; multiple inputs
//   * proc= != owner; multiple returns; multiple inputs

// TODO: test calling of function
//   * multiple inputs; data not present; throws exception
//   * multiple inputs; multiple output; call is successful
//   * call throws

// TODO: test signal emmission

    Adder adder;
    adder.SetValue( 5 );



}


