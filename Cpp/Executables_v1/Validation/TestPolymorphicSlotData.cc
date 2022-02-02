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

KTLOGGER(testslotdata, "TestPolymorphicSlotData")

int main()
{
    try
    {
        KTTestProcessorE tpE;

        KTINFO(testslotdata, "Connecting derived-1 to derived-1 and derived-2 to derived-2");
        // for simplicity the signal and slot are in the same processor, but that need not be, of course
        tpE.ConnectASlot("derived-1", &tpE, "derived-1" );
        tpE.ConnectASlot("derived-2", &tpE, "derived-2" );

        KTINFO(testslotdata, "Emitting signals");
        tpE.EmitSignals();

        KTINFO(testslotdata, "Tests complete");
    }
    catch( boost::exception& e )
    {
        KTERROR( testslotdata, "An exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
}

