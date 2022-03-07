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

} /* namespace Nymph */
#endif /* KTTESTPROCESSOR_HH_ */
