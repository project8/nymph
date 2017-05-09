/*
 * TestProcessorToolboxMultithreaded.cc
 *
 *  Created on: May 8, 2017
 *      Author: N.S. Oblath
 */

#include "KTProcessorToolbox.hh"

#include "KTLogger.hh"

using namespace Nymph;

KTLOGGER( testptlog, "TestProcessorToolbox" )

int main()
{
    KTINFO( testptlog, "Preparing to run" );

    KTProcessorToolbox ptb;

    // add the primary processor, thread 1
    if( ! ptb.AddProcessor( "test-p-proc", "tpp1" ) )
    {
        KTERROR( testptlog, "Unable to create test primary processor (thread 1)" );
        return -1;
    }

    // add processor b, thread 1
    if( ! ptb.AddProcessor( "test-proc-b", "tp1" ) )
    {
        KTERROR( testptlog, "Unable to create test processor b (thread 1)" );
        return -1;
    }

    // add the primary processor, thread 2
    if( ! ptb.AddProcessor( "test-p-proc", "tpp2" ) )
    {
        KTERROR( testptlog, "Unable to create test primary processor (thread 2)" );
        return -1;
    }

    // add processor b, thread 2
    if( ! ptb.AddProcessor( "test-proc-b", "tp2" ) )
    {
        KTERROR( testptlog, "Unable to create test processor b (thread 2)" );
        return -1;
    }

    // make connections, thread 1
    if( ! ptb.MakeConnection( "tpp1:the-signal", "tp1:first-slot" ) )
    {
        KTERROR( testptlog, "Unable to connect tpp1 to tp1" );
        return -1;
    }

    // make connections, thread 2
    if( ! ptb.MakeConnection( "tpp2:the-signal", "tp2:first-slot" ) )
    {
        KTERROR( testptlog, "Unable to connect tpp2 to tp2" );
        return -1;
    }

    // set the run queue
    if( ! ptb.PushBackToRunQueue( {"tpp1", "tpp2"} ) )
    {
        KTERROR( testptlog, "Unable to add {tpp1, tpp2} to the run queue" );
        return -1;
    }

    // go!
    KTINFO( testptlog, "Starting asynchronous run" );
    ptb.AsyncRun();

    // wait for run completion, and ignore breakpoints
    while( ptb.WaitForBreak() )
    {
        KTINFO( testptlog, "Reached breakpoint; continuing" );
        ptb.Continue();
    }

    KTINFO( testptlog, "Processing complete" );

    KTINFO( testptlog, "Tests complete" );

    return 0;
}