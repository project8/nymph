/*
 * TestProcessor.cc
 *
 *  Created on: Sept 12, 2019
 *      Author: N.S. Oblath
 */

#include "TestControllerClasses.hh"
#include "TestProcessorClasses.hh"

#include "logger.hh"

#include "catch.hpp"

LOGGER( testlog, "TestProcessor" );

TEST_CASE( "processor", "[signal],[slot],[processor]" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    // need a controller to exist
    CIQThrowController tpController;

    //SharedControl::get_instance()->Reset();

    TestProc tpProcessor;

    SECTION( "Configuration" )
    {
        REQUIRE( tpProcessor.GetValue() == 0 );

        // setup the config parameter
        scarab::param_node config;
        int configValue = 5;
        config.add( "value", configValue );

        // perform configuration with no value for "string"
        REQUIRE_NOTHROW( tpProcessor.Configure( config ) );
        // check that the value was set correctly
        REQUIRE( tpProcessor.GetValue() == configValue );

        // peform configuration with an illegal value for "string"
        config.add( "string", "illegal value" );
        REQUIRE_THROWS_AS( tpProcessor.Configure( config ), ConfigException );
        try
        {
            tpProcessor.Configure( config );
        }
        catch( const ConfigException& e )
        {
            PrintException(e);
        }
        
        // perform configuration with a legal value for "string"
        config["string"]().set( "ok value 1" );
        REQUIRE_NOTHROW( tpProcessor.Configure( config ) );
        REQUIRE( tpProcessor.StringValue() == "ok 1" );

    }

    SECTION( "Signals and Slots" )
    {
        // check that the processor has the right number of signals and slots registered
        REQUIRE( tpProcessor.Signals().size() == 2 );
        REQUIRE( tpProcessor.Slots().size() == 3 );

        //**************************
        // Testing signal->slot
        //**************************

        // this is a slot using a lambda function to set a variable
        int lambdaValue = 0;
        Slot< int > lambdaSlot( "lambdaSlot", [&lambdaValue](int input){ lambdaValue = input; } );
        tpProcessor.RegisterSlot( "lambdaSlot", &lambdaSlot );

        // this slot is a member function of an object other than the processor
        TestSlotOwner slotOwner;
        Slot< int > slotOwnerSlot( "slotOwnerSlot", &tpProcessor, &slotOwner, &TestSlotOwner::TestSlotFunc );

        // check initial value
        REQUIRE( slotOwner.fValue == 0 );
        REQUIRE( lambdaValue == 0 );

        // we've added a couple more slots
        REQUIRE( tpProcessor.Slots().size() == 5 );

        // no connections yet
        REQUIRE( tpProcessor.Signals().at("value")->Connections().size() == 0 );
        REQUIRE( tpProcessor.Slots().at("value")->Connections().size() == 0 );

        // make a connection
        tpProcessor.ConnectASlot( "value", tpProcessor, "value" );

        REQUIRE( tpProcessor.Signals().at("value")->Connections().size() == 1 );
        REQUIRE( tpProcessor.Slots().at("value")->Connections().size() == 1 );

        // repeat connection should be ignored
        tpProcessor.ConnectASlot( "value", tpProcessor, "value" );

        REQUIRE( tpProcessor.Signals().at("value")->Connections().size() == 1 );
        REQUIRE( tpProcessor.Slots().at("value")->Connections().size() == 1 );

        // connect the lambda slot to the signal
        tpProcessor.ConnectASignal( tpProcessor, "value", "lambdaSlot" );

        REQUIRE( tpProcessor.Signals().at("value")->Connections().size() == 2 );
        REQUIRE( tpProcessor.Slots().at("value")->Connections().size() == 1 );
        REQUIRE( tpProcessor.Slots().at("lambdaSlot")->Connections().size() == 1 );

        // connect the exteranlly-owned slot to the signal
        Processor::ConnectSignalToSlot( tpProcessor.Signals().at("value"), tpProcessor.Slots().at("slotOwnerSlot") );

        REQUIRE( tpProcessor.Signals().at("value")->Connections().size() == 3 );
        REQUIRE( tpProcessor.Slots().at("value")->Connections().size() == 1 );
        REQUIRE( tpProcessor.Slots().at("slotOwnerSlot")->Connections().size() == 1 );

        // now have "signal" connected to three slots:
        //     value --> value
        //     value --> lambdaSlot
        //     value --> slotOwnerSlot

        // emit the signal
        int newValue = 10;
        LWARN( testlog, "do breakpoint in signal second-value? " << tpProcessor.Signals().at("second-value")->GetDoBreakpoint() );
        tpProcessor.ValueSig().Emit( newValue );

        // check that all values have been changed
        REQUIRE( tpProcessor.GetValue() == newValue );
        REQUIRE( slotOwner.fValue == newValue );
        REQUIRE( lambdaValue == newValue );

        //*********************************
        // testing slot->signal->slot
        //*********************************

        // check initial value
        REQUIRE( tpProcessor.GetSecondValue() == 0 );

        //REQUIRE( tpProcessor.Slots().at("second-value")->SignalsUsed().size() == 1 );
        //REQUIRE( tpProcessor.Slots().at("second-value")->SignalsUsed()[0] == &tpProcessor.SecondValueSig() );

        tpProcessor.ConnectASlot( "second-value", tpProcessor, "second-value-2" );

        // now have second-value (slot) emits second-value --> second-value-2

        REQUIRE( tpProcessor.Signals().at("second-value")->Connections().size() == 1 );
        REQUIRE( tpProcessor.Slots().at("second-value-2")->Connections().size() == 1 );

        tpProcessor.SecondValueSlotFunction( newValue );

        REQUIRE( tpProcessor.GetSecondValue() == newValue );
    }

}
