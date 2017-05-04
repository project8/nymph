/*
 * KTTestProcessor.cc
 *
 *  Created on: Aug 15, 2012
 *      Author: nsoblath
 */

#include "KTTestProcessor.hh"

#include "KTLogger.hh"

namespace Nymph
{
    KTLOGGER(testsiglog, "KTTestProcessor")

    KTTestProcessorA::KTTestProcessorA() :
            fTheSignal("the-signal", this)
    {
    }

    KTTestProcessorA::~KTTestProcessorA()
    {
    }

    bool KTTestProcessorA::Configure(const scarab::param_node*)
    {
        return true;
    }

    void KTTestProcessorA::EmitSignals(int value)
    {
        fTheSignal(value);
        return;
    }




    KTTestProcessorB::KTTestProcessorB() :
            fSlot1("first-slot", this, &KTTestProcessorB::SlotFunc1, {}),
            fSlot2("second-slot", this, &KTTestProcessorB::SlotFunc2, {})
    {
    }

    KTTestProcessorB::~KTTestProcessorB()
    {
    }

    bool KTTestProcessorB::Configure(const scarab::param_node*)
    {
        return true;
    }

    void KTTestProcessorB::SlotFunc1(int input)
    {
        KTINFO(testsiglog, "Slot1: input is " << input);
        return;
    }

    void KTTestProcessorB::SlotFunc2(int input)
    {
        KTINFO(testsiglog, "Slot2: twice input is " << 2 * input);
        return;
    }



} /* namespace Nymph */
