/*
 * TestProcessor.cc
 *
 *  Created on: Sept 12, 2019
 *      Author: N.S. Oblath
 */

#include "Processor.hh"
#include "Signal.hh"
#include "Slot.hh"

#include "catch.hpp"

namespace Nymph
{
    // concrete processor class that we can test
    // implements Configure() and has its own signal and slot
    class TestProc : public Processor
    {
        public:
            TestProc() :
                    Processor( "test" ),
                    fValue( 0 ),
                    fValueSig( "value", this ),
                    fValueSlot( "value", this, &TestProc::SetValue )
            {}

            ~TestProc()
            {}

            void Configure( const scarab::param_node& node )
            {
                fValue = node.get_value( "value", fValue );
                return;
            }

            MEMVAR( int, Value );

            Signal< int >& ValueSig()
            {
                return fValueSig;
            }

        private:
            Signal< int > fValueSig;
            Slot< int > fValueSlot;
    };

    // external slot function owner
    struct TestSlotOwner
    {
        int fValue = 0;
        void TestSlotFunc( int input )
        {
            fValue = input;
            return;
        }
    };
}

TEST_CASE( "processor", "[signal],[slot],[processor]" )
{
    using namespace Nymph;

    TestProc tester;

    SECTION( "Configuration" )
    {
        REQUIRE( tester.GetValue() == 0 );

        // setup the config parameter
        scarab::param_node config;
        int configValue = 5;
        config.add( "value", configValue );

        // perform configuration
        tester.Configure( config );

        // check that the value was set correctly
        REQUIRE( tester.GetValue() == configValue );
    }

    SECTION( "Signals and Slots" )
    {
        // check that the processor has one slot and one signal
        REQUIRE( tester.Signals().size() == 1 );
        REQUIRE( tester.Slots().size() == 1 );

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
        REQUIRE( tester.Slots().size() == 3 );

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

        // emit the signal
        int newValue = 10;
        tester.ValueSig().Emit( newValue );

        // check that all values have been changed
        REQUIRE( tester.GetValue() == newValue );
        REQUIRE( slotOwner.fValue == newValue );
        REQUIRE( lambdaValue == newValue );
    }

}
