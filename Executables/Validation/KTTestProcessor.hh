/*
 * KTTestProcessor.hh
 *
 *  Created on: Aug 15, 2012
 *      Author: nsoblath
 */

#ifndef KTTESTPROCESSOR_HH_
#define KTTESTPROCESSOR_HH_

#include "KTProcessor.hh"
#include "KTSlot.hh"

#include "KTTestData.hh"

namespace Nymph
{

    /*!
     * A simple test processor that emits a signal with an integer argument
     */
    class KTTestProcessorA : public KTProcessor
    {
        public:
            KTTestProcessorA( const std::string& name = "test-proc-a" );
            virtual ~KTTestProcessorA();

            bool Configure(const scarab::param_node* node);

            void EmitSignals(int);

        private:
            KTSignal< int > fTheSignal;
    };

    /*!
     * A simple test processor that has two slots with integer arguments
     */
    class KTTestProcessorB : public KTProcessor
    {
        public:
            KTTestProcessorB( const std::string& name = "test-proc-b" );
            virtual ~KTTestProcessorB();

            bool Configure(const scarab::param_node* node);

            void SlotFunc1(int);
            void SlotFunc2(int);

        private:
            KTSlot< int > fSlot1;
            KTSlot< int > fSlot2;

            KTSlotWrapper* fSlot1Wrapper;
            KTSlotWrapper* fSlot2Wrapper;
    };

    /*!
     * A simple test processor that has one slot that throws an exception
     */
    class KTTestProcessorC : public KTProcessor
    {
        public:
            KTTestProcessorC( const std::string& name = "test-proc-c" );
            virtual ~KTTestProcessorC();

            bool Configure(const scarab::param_node* node);

            void SlotFunc1(int);

        private:
            KTSlot< int > fSlot1;
    };


//    class KTTestData;

    /*!
     * A simple test processor that has a slot for KTTestData
     */
    class KTTestProcessorD : public KTProcessor
    {
        public:
            KTTestProcessorD( const std::string& name = "test-proc-d" );
            virtual ~KTTestProcessorD();

            bool Configure(const scarab::param_node* node);

            // Creates a KTTestData object, sets the awesomeness to the given value, and emits fSignal
            void EmitSignal(bool isAwesome = true);

            bool SlotFunc(KTTestData& data);

        private:
            KTSlotData< KTTestData > fDataSlot;

            KTSignalData fDataSignal;
    };


//    template< class XDerivedDataType >
//    class KTTestPolyDataBase< XDerivedDataType >;
//    class KTTestDerived1Data;
//    class KTTestDerived2Data;

    /*!
     * A simple test processor for testing polymorphic data, signals, and slots
     */
    class KTTestProcessorE : public KTProcessor
    {
        public:
            KTTestProcessorE( const std::string& name = "test-proc-d" );
            virtual ~KTTestProcessorE();

            bool Configure(const scarab::param_node* node);

            void EmitSignals();

            template< class XDerivedDataType >
            bool BaseSlotFunc(KTTestPolyDataBase< XDerivedDataType >& data);

        private:
            KTSlotData< KTTestDerived1Data > fDerived1DataSlot;
            KTSlotData< KTTestDerived2Data > fDerived2DataSlot;

            KTSignalData fDerived1DataSignal;
            KTSignalData fDerived2DataSignal;

            // utility function to avoid putting a logger in the header
            void PrintFunniness( unsigned funniness );
    };

    template< class XDerivedDataType >
    bool KTTestProcessorE::BaseSlotFunc(KTTestPolyDataBase< XDerivedDataType >& data)
    {
        PrintFunniness( data.GetFunniness() );
        return true;
    }


} /* namespace Nymph */
#endif /* KTTESTPROCESSOR_HH_ */
