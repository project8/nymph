/*
 * TestSlotData.cc
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */


#include "KTTestData.hh"
#include "KTTestProcessor.hh"
#include "KTLogger.hh"

using namespace Nymph;

KTLOGGER(testslotdata, "TestSlotData")

int main()
{
    try
    {
        KTTestProcessorD tpD;

        KTINFO(testslotdata, "Connecting test-data-signal to test-data-slot");
        // for simplicity the signal and slot are in the same processor, but that need not be, of course
        tpD.ConnectASlot("test-data-signal", &tpD, "test-data-slot" );

        KTINFO(testslotdata, "Emitting signals");
        KTINFO(testslotdata, "First test signal: true");
        tpD.EmitSignal(true);
        KTINFO(testslotdata, "Second test signal: false");
        tpD.EmitSignal(false);

        KTINFO(testslotdata, "Tests complete");
    }
    catch( boost::exception& e )
    {
        KTERROR( testslotdata, "An exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
}

