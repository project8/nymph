/*
 * TestProc.hh
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"
#include "Processor.hh"
#include "Signal.hh"
#include "Slot.hh"


namespace Nymph
{
    // concrete processor class that we can test
    // implements Configure() and has its own signal and slot
    class TestProc : public Processor
    {
        public:
            TestProc( const std::string& name = "test" ) :
                    Processor( name ),
                    fValue( 0 ),
                    fSecondValue( 0 ),
                    fValueSig( "value", this ),
                    fValueSlot( "value", this, &TestProc::SetValue ),
                    fSecondValueSig( "second-value", this ),
                    fSecondValueSlot( "second-value", this, &TestProc::SecondValueSlotFunction, {"second-value"} ),
                    fSecondValueSlot2( "second-value-2", this, &TestProc::SetSecondValue )
            {}

            virtual ~TestProc()
            {}

            void Configure( const scarab::param_node& node )
            {
                fValue = node.get_value( "value", fValue );
                return;
            }

            MEMVAR( int, Value );
            MEMVAR( int, SecondValue );

            void SecondValueSlotFunction( int newValue )
            {
                fSecondValueSig( newValue );
            }

            MEMVAR_REF( Signal< int >, ValueSig );
            MEMVAR_REF( Slot< int >, ValueSlot );

            MEMVAR_REF( Signal< int >, SecondValueSig );
            MEMVAR_REF( Slot< int >, SecondValueSlot );
            MEMVAR_REF( Slot< int >, SecondValueSlot2 );

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


    // concrete primary processor class that we can test
    // implements Configure(), Run(), and has its own signal and slot
    class TestPrimaryProc : public PrimaryProcessor
    {
        public:
            TestPrimaryProc( const std::string& name = "test" ) :
                    PrimaryProcessor( name ),
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
