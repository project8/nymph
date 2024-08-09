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

    KT_REGISTER_PROCESSOR(KTTestProcessorA, "test-proc-a");

    KTTestProcessorA::KTTestProcessorA(const std::string& name) :
            KTProcessor(name),
            fTheSignal()
    {
        RegisterSignal("the_signal", &fTheSignal);
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


    KT_REGISTER_PROCESSOR(KTTestProcessorB, "test-proc-b");

    KTTestProcessorB::KTTestProcessorB(const std::string& name) : 
            KTProcessor(name)
    {
        RegisterSlot("first_slot", this, &KTTestProcessorB::Slot1);
        RegisterSlot("second_slot", this, &KTTestProcessorB::Slot2);
    }

    KTTestProcessorB::~KTTestProcessorB()
    {
    }

    bool KTTestProcessorB::Configure(const scarab::param_node*)
    {
        return true;
    }

    void KTTestProcessorB::Slot1(int input)
    {
        KTINFO(testsiglog, "Slot1: input is " << input);
        return;
    }

    void KTTestProcessorB::Slot2(int input)
    {
        KTINFO(testsiglog, "Slot2: twice input is " << 2*input);
        return;
    }



} /* namespace Nymph */
