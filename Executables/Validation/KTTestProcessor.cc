/*
 * KTTestProcessor.cc
 *
 *  Created on: Aug 15, 2012
 *      Author: nsoblath
 */

#include "KTTestProcessor.hh"

#include "KTException.hh"
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



    KTTestProcessorC::KTTestProcessorC() :
            fSlot1("first-slot", this, &KTTestProcessorC::SlotFunc1, {})
    {
    }

    KTTestProcessorC::~KTTestProcessorC()
    {
    }

    bool KTTestProcessorC::Configure(const scarab::param_node*)
    {
        return true;
    }

    void KTTestProcessorC::SlotFunc1(int input)
    {
        KTThreadReference* ref = fSlot1.GetSlotWrapper()->GetThreadRef();

        try
        {
            KTINFO(testsiglog, "Slot1: input is " << input);
            throw KTException() << "A HUGE problem occurred!!!! (just kidding, this is the expected result)";
        }
        catch( ... )
        {
            ref->fDataPtrRet.set_exception( std::current_exception() );
        }
        return;
    }


} /* namespace Nymph */
