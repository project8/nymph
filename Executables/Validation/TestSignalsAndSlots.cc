/*
 * TestSignalsAndSlots.cc
 *
 *  Created on: Aug 15, 2012
 *      Author: nsoblath
 */


#include "KTTestProcessor.hh"
#include "KTLogger.hh"

using namespace Nymph;

KTLOGGER(testsiglog, "TestSignalsAndSlots")

int main()
{
    try
    {
        KTTestProcessorA tpA;
        KTTestProcessorB tpB;

        /* Basic Test
        KTSignalWrapper* signalPtr = tpA.GetSignal("the_signal");
        KTSlotWrapper* slot1Ptr = tpB.GetSlot("first_slot");

        slot1Ptr->SetConnection(signalPtr);

        tpA.EmitSignals(5);

        slot1Ptr->Disconnect();

        return 0;
        */

        /* More Complicated Test */
        KTINFO(testsiglog, "Connecting the-signal to first-slot and second-slot");
        tpA.ConnectASlot("the-signal", &tpB, "first-slot", 20);
        tpB.ConnectASignal(&tpA, "the-signal", "second-slot", 10);

        KTINFO(testsiglog, "Emitting signals");
        KTINFO(testsiglog, "First test signal: 5");
        tpA.EmitSignals(5);
        KTINFO(testsiglog, "Second test signal: 18");
        tpA.EmitSignals(18);

        KTINFO(testsiglog, "Tests complete");
    }
    catch( boost::exception& e )
    {
        KTERROR( testsiglog, "An exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
    /**/
}

