/*
 * TestProcessorClasses.hh
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#include "TestDataClasses.hh"

#include "Processor.hh"
#include "SignalData.hh"
#include "SlotData.hh"


namespace Nymph
{
    // This processor has a data slot that takes TestData1 as input and has no output
    class Adder : public Processor
    {
        public:
            Adder( const std::string& name = "adder" ) :
                    Processor( name ),
                    fValue( 0 ),
                    fAddSignal( "add", this ),
                    fAddSlotWithSig( "add-sig", this, &Adder::Add, &fAddSignal ),
                    fAddSlotNoSig( "add-no-sig", this, &Adder::Add )
            {}

            virtual ~Adder()
            {}

            void Configure( const scarab::param_node& node )
            {
                fValue = node.get_value( "value", fValue );

                return;
            }

            MEMVAR( int, Value );

        public:
            void Add( TestData1& data )
            {
                data.SetIValue1( data.GetIValue1() + fValue );
                data.SetIValue2( data.GetIValue2() + fValue );
                return;
            }

        public:
            SignalData fAddSignal;

            SlotData< In<TestData1>, Out<> > fAddSlotWithSig;
            SlotData< In<TestData1>, Out<> > fAddSlotNoSig;

    };

}
