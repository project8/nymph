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
//#include "pybind11/pybind11.h"


//using namespace py;

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

            bool Configure(const scarab::param_node& node);

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

            bool Configure(const scarab::param_node& node);

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

            bool Configure(const scarab::param_node& node);

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

            bool Configure(const scarab::param_node& node);

            // Creates a KTTestData object, sets the awesomeness to the given value, and emits fSignal
            void EmitSignal(bool isAwesome = true);

            void AnalysisFunc(const KTTestData& data);

        private:
            KTSlotData< void, KTTestDataExt > fDataSlot;

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

            bool Configure(const scarab::param_node& node);

            void EmitSignals();

            void BaseAnalysisFunc(const KTTestBaseData& data);

        private:
            KTSlotData< void, KTTestDerived1DataExt > fDerived1DataSlot;
            KTSlotData< void, KTTestDerived2DataExt > fDerived2DataSlot;

            KTSignalData fDerived1DataSignal;
            KTSignalData fDerived2DataSignal;
    };


    class KTWrapProcessor : public KTProcessor
    {
        public:
            KTWrapProcessor( const std::string& name = "base-wrap-processor" );
            virtual ~KTWrapProcessor();

            bool Configure(const scarab::param_node& node);

            void SlotFunc(int);
            virtual void WrapFunction(int input);

        private:
            KTSlot< int > fSlot;

    };

} /* namespace Nymph */
#endif /* KTTESTPROCESSOR_HH_ */
