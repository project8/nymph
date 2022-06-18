/*
 * TestProcessorClasses.hh
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_TESTING_TESTDATAPROCESSORCLASSES_HH
#define NYMPH_TESTING_TESTDATAPROCESSORCLASSES_HH

#include "TestDataClasses.hh"

#include "Exception.hh"
#include "Processor.hh"
#include "SignalData.hh"
#include "SlotData.hh"

#include "logger.hh"

LOGGER( tdpclog_h, "TestDataProcessorClasses" );

namespace NymphTesting
{

    // external slot function owner
    struct MultiplyFuncOwner
    {
        double fValue = 2.5;
        void Multiply( const TestData1& intData, TestData2& doubleData )
        {
            doubleData.SetDValue1( (double)intData.GetIValue1() * fValue );
            doubleData.SetDValue2( (double)intData.GetIValue2() * fValue );
            return;
        }
    };

    // This processor has a data slot that takes TestData1 as input and has no output (TestData1 is modified in place)
    class Adder : public Nymph::Processor
    {
        public:
            Adder( const std::string& name = "adder" ) :
                    Processor( name ),
                    fAddValue( 0 ),
                    fAddSignal( "add", this ),
                    fAddSlotWithSig( "add-sig", this, &Adder::Add, &fAddSignal ),
                    fAddSlotNoSig( "add-no-sig", this, &Adder::Add ),
                    fMultiplySlot( "multiply", this, &fMultFunc, &MultiplyFuncOwner::Multiply ),
                    fIToDSlot( "i-to-d", this, &Adder::IntToDouble ),
                    fPrintSlot( "print", this, &Adder::Print ),
                    fJustThrowsSlot( "just-throws", this, &Adder::JustThrows )
            {}

            virtual ~Adder()
            {}

            void Configure( const scarab::param_node& node )
            {
                fAddValue = node.get_value( "to-add", fAddValue );

                fMultFunc.fValue = node.get_value( "to-mult", fMultFunc.fValue );

                return;
            }

            MEMVAR( int, AddValue );

        public:
            void Add( TestData1& data )
            {
                data.SetIValue1( data.GetIValue1() + fAddValue );
                data.SetIValue2( data.GetIValue2() + fAddValue );
                return;
            }

            void Print( const TestData1& data )
            {
                LPROG( tdpclog_h, "Value 1: " << data.GetIValue1() );
                LPROG( tdpclog_h, "Value 2: " << data.GetIValue2() );
                return;
            }

            void IntToDouble( const TestData1& intData, TestData2& doubleData )
            {
                doubleData.SetDValue1( (double)intData.GetIValue1() );
                doubleData.SetDValue2( (double)intData.GetIValue2() );
                return;
            }

            void JustThrows( TestData1& )
            {
                throw CREATE_EXCEPT_HERE( Nymph::Exception );
            }

        protected:
            MultiplyFuncOwner fMultFunc;

        public:
            Nymph::SignalData fAddSignal;

            USING_NYMPH_IN_OUT; // avoids needing to prepend In and Out with Nymph::

            Nymph::SlotData< In<TestData1>, Out<> > fAddSlotWithSig;
            Nymph::SlotData< In<TestData1>, Out<> > fAddSlotNoSig;

            Nymph::SlotData< In<const TestData1>, Out<TestData2> > fMultiplySlot;

            Nymph::SlotData< In<const TestData1>, Out<TestData2> > fIToDSlot;

            Nymph::SlotData< In<const TestData1>, Out<> > fPrintSlot;

            Nymph::SlotData< In<TestData1>, Out<> > fJustThrowsSlot;

    };

}

#endif /* NYMPH_TESTING_TESTDATAPROCESSORCLASSES_HH */
