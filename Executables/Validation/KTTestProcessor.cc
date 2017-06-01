/*
 * KTTestProcessor.cc
 *
 *  Created on: Aug 15, 2012
 *      Author: nsoblath
 */

#include "KTTestProcessor.hh"

#include "KTException.hh"
#include "KTLogger.hh"

#include "KTTestData.hh"

namespace Nymph
{
    KTLOGGER(testsiglog, "KTTestProcessor")

    KT_REGISTER_PROCESSOR(KTTestProcessorA, "test-proc-a");

    KTTestProcessorA::KTTestProcessorA( const std::string& name ) :
            KTProcessor( name ),
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


    KT_REGISTER_PROCESSOR(KTTestProcessorB, "test-proc-b");

    KTTestProcessorB::KTTestProcessorB( const std::string& name ) :
            KTProcessor( name ),
            fSlot1("first-slot", this, &KTTestProcessorB::SlotFunc1, {}),
            fSlot2("second-slot", this, &KTTestProcessorB::SlotFunc2, {})
    {
        fSlot1Wrapper = GetSlot( "first-slot" );
        fSlot2Wrapper = GetSlot( "second-slot" );
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
        fSlot1Wrapper->GetThreadRef()->Break( KTDataPtr(), fSlot1Wrapper->GetDoBreakpoint());
        return;
    }

    void KTTestProcessorB::SlotFunc2(int input)
    {
        KTINFO(testsiglog, "Slot2: twice input is " << 2 * input);
        fSlot2Wrapper->GetThreadRef()->Break( KTDataPtr(), fSlot2Wrapper->GetDoBreakpoint());
        return;
    }


    KT_REGISTER_PROCESSOR(KTTestProcessorC, "test-proc-c");

    KTTestProcessorC::KTTestProcessorC( const std::string& name ) :
            KTProcessor( name ),
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
        std::shared_ptr< KTThreadReference > ref = fSlot1.GetSlotWrapper()->GetThreadRef();

        KTINFO(testsiglog, "Slot1: input is " << input);
        BOOST_THROW_EXCEPTION( KTException() << "A HUGE problem occurred!!!! (just kidding, this is the expected result)" << eom );

        return;
    }


    KT_REGISTER_PROCESSOR(KTTestProcessorD, "test-proc-d");

    KTTestProcessorD::KTTestProcessorD( const std::string& name ) :
            KTProcessor( name ),
            fSlot("test-data", this, &KTTestProcessorD::SlotFunc, {})
    {
    }

    KTTestProcessorD::~KTTestProcessorD()
    {
    }

    bool KTTestProcessorD::Configure(const scarab::param_node*)
    {
        return true;
    }

    bool KTTestProcessorD::SlotFunc(KTTestData& data)
    {
        KTINFO(testsiglog, "Is the data awesome? " << data.GetIsAwesome());
        return true;
    }

} /* namespace Nymph */
