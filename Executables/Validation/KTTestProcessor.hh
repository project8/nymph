/*
 * KTTestProcessor.hh
 *
 *  Created on: Aug 15, 2012
 *      Author: nsoblath
 */

#ifndef KTTESTPROCESSOR_HH_
#define KTTESTPROCESSOR_HH_

#include "KTProcessor.hh"

#include "KTSignal.hh"
#include "KTSlot.hh"

namespace Nymph
{

    /*!
     * A simple test processor that emits a signal with an integer argument
     */
    class KTTestProcessorA : public KTProcessor
    {
        public:
            KTTestProcessorA();
            virtual ~KTTestProcessorA();

            bool Configure(const scarab::param_node* node);

            void EmitSignals(int);

        private:
            KTSignal< int > fTheSignal;
    };

    /*!
     * A simple test procesor that has two slots with integer arguments
     */
    class KTTestProcessorB : public KTProcessor
    {
        public:
            KTTestProcessorB();
            virtual ~KTTestProcessorB();

            bool Configure(const scarab::param_node* node);

            void SlotFunc1(int);
            void SlotFunc2(int);

        private:
            KTSlot< int > fSlot1;
            KTSlot< int > fSlot2;
};


} /* namespace Nymph */
#endif /* KTTESTPROCESSOR_HH_ */
