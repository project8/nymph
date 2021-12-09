/*
 * TestPrimaryProcessor.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"
#include "Signal.hh"
#include "Slot.hh"

#include "logger.hh"

#include "catch.hpp"

LOGGER( testlog, "TestPrimaryProcessor" );

namespace Nymph
{
    // concrete processor class that we can test
    // implements Configure() and has its own signal and slot
    class TestPrimaryProc : public PrimaryProcessor
    {
        public:
            TestPrimaryProc() :
                    PrimaryProcessor( "test" ),
                    fNewValue( 10 ),
                    fValue( 0 ),
                    fValueSig( "value", this ),
                    fValueSlot( "value", this, &TestPrimaryProc::SetValue )
            {}

            virtual ~TestPrimaryProc()
            {}

            void Configure( const scarab::param_node& )
            {
                return;
            }

            bool Run()
            {
                fValueSig( fNewValue );
                return true;
            }

            MEMVAR( int, NewValue );
            MEMVAR( int, Value );

            MEMVAR_REF( Signal< int >, ValueSig );
            MEMVAR_REF( Slot< int >, ValueSlot );

    };
}

TEST_CASE( "primary_processor", "[primary_processor]" )
{
    using namespace Nymph;

    TestPrimaryProc tester;

    tester.ConnectASlot( "value", tester, "value" );

    tester();

    REQUIRE( tester.GetValue() == tester.GetNewValue() );

    //REQUIRE( tester.ValueSig().GetControl() == &control );
}
