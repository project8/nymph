/*
 * TestProcessor.cc
 *
 *  Created on: Sept 12, 2019
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

#include "logger.hh"

#include "catch.hpp"

LOGGER( testlog, "TestProcessor" );

REGISTER_PROCESSOR(Nymph, TestProc, "test-proc");


TEST_CASE( "processor", "[signal],[slot],[processor]" )
{
    using namespace Nymph;

    //SharedControl::get_instance()->Reset();

    TestProc tester;

    SECTION( "Configuration" )
    {
        REQUIRE( tester.GetValue() == 0 );

        // setup the config parameter
        scarab::param_node config;
        int configValue = 5;
        config.add( "value", configValue );

        // perform configuration with no value for "string"
        REQUIRE_NOTHROW( tester.Configure( config ) );
        // check that the value was set correctly
        REQUIRE( tester.GetValue() == configValue );

        // peform configuration with an illegal value for "string"
        config.add( "string", "illegal value" );
        REQUIRE_THROWS_AS( tester.Configure( config ), ConfigException );
        try
        {
            tester.Configure( config );
        }
        catch( const ConfigException& e )
        {
            PrintException(e);
        }
        
        // perform configuration with a legal value for "string"
        config["string"]().set( "ok value 1" );
        REQUIRE_NOTHROW( tester.Configure( config ) );
        REQUIRE( tester.StringValue() == "ok 1" );

    }

    SECTION( "Signals and Slots" )
    {
        // check that the processor has the right number of signals and slots registered
        REQUIRE( tester.Signals().size() == 2 );
        REQUIRE( tester.Slots().size() == 3 );

        //**************************
        // Testing signal->slot
        //**************************

        // this is a slot using a lambda function to set a variable
        int lambdaValue = 0;
        Slot< int > lambdaSlot( "lambdaSlot", [&lambdaValue](int input){ lambdaValue = input; } );
        tester.RegisterSlot( "lambdaSlot", &lambdaSlot );

        // this slot is a member function of an object other than the processor
        TestSlotOwner slotOwner;
        Slot< int > slotOwnerSlot( "slotOwnerSlot", &tester, &slotOwner, &TestSlotOwner::TestSlotFunc );

        // check initial value
        REQUIRE( slotOwner.fValue == 0 );
        REQUIRE( lambdaValue == 0 );

        // we've added a couple more slots
        REQUIRE( tester.Slots().size() == 5 );

        // no connections yet
        REQUIRE( tester.Signals().at("value")->Connections().size() == 0 );
        REQUIRE( tester.Slots().at("value")->Connections().size() == 0 );

        // make a connection
        tester.ConnectASlot( "value", tester, "value" );

        REQUIRE( tester.Signals().at("value")->Connections().size() == 1 );
        REQUIRE( tester.Slots().at("value")->Connections().size() == 1 );

        // repeat connection should be ignored
        tester.ConnectASlot( "value", tester, "value" );

        REQUIRE( tester.Signals().at("value")->Connections().size() == 1 );
        REQUIRE( tester.Slots().at("value")->Connections().size() == 1 );

        // connect the lambda slot to the signal
        tester.ConnectASignal( tester, "value", "lambdaSlot" );

        REQUIRE( tester.Signals().at("value")->Connections().size() == 2 );
        REQUIRE( tester.Slots().at("value")->Connections().size() == 1 );
        REQUIRE( tester.Slots().at("lambdaSlot")->Connections().size() == 1 );

        // connect the exteranlly-owned slot to the signal
        Processor::ConnectSignalToSlot( tester.Signals().at("value"), tester.Slots().at("slotOwnerSlot") );

        REQUIRE( tester.Signals().at("value")->Connections().size() == 3 );
        REQUIRE( tester.Slots().at("value")->Connections().size() == 1 );
        REQUIRE( tester.Slots().at("slotOwnerSlot")->Connections().size() == 1 );

        // now have "signal" connected to three slots:
        //     value --> value
        //     value --> lambdaSlot
        //     value --> slotOwnerSlot

        // emit the signal
        int newValue = 10;
        tester.ValueSig().Emit( newValue );

        // check that all values have been changed
        REQUIRE( tester.GetValue() == newValue );
        REQUIRE( slotOwner.fValue == newValue );
        REQUIRE( lambdaValue == newValue );

        //*********************************
        // testing slot->signal->slot
        //*********************************

        // check initial value
        REQUIRE( tester.GetSecondValue() == 0 );

        //REQUIRE( tester.Slots().at("second-value")->SignalsUsed().size() == 1 );
        //REQUIRE( tester.Slots().at("second-value")->SignalsUsed()[0] == &tester.SecondValueSig() );

        tester.ConnectASlot( "second-value", tester, "second-value-2" );

        // now have second-value (slot) emits second-value --> second-value-2

        REQUIRE( tester.Signals().at("second-value")->Connections().size() == 1 );
        REQUIRE( tester.Slots().at("second-value-2")->Connections().size() == 1 );

        tester.SecondValueSlotFunction( newValue );

        REQUIRE( tester.GetSecondValue() == newValue );
    }

}
