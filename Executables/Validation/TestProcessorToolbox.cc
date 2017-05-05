/*
 * TestProcessorToolbox.cc
 *
 *  Created on: May 4, 2017
 *      Author: N.S. Oblath
 */

#include "KTProcessorToolbox.hh"

#include "KTLogger.hh"

#include <thread>

using namespace Nymph;

KTLOGGER( testptlog, "TestProcessorToolbox" )

int main()
{
    KTINFO( testptlog, "Preparing to run" );

    KTProcessorToolbox ptb;

    if( ! ptb.AddProcessor( "test-p-proc", "tpp" ) )
    {
        KTERROR( testptlog, "Unable to create test primary processor" );
        return -1;
    }

    if( ! ptb.AddProcessor( "test-proc-b", "tp" ) )
    {
        KTERROR( testptlog, "Unable to create test processor b" );
        return -1;
    }

    if( ! ptb.MakeConnection( "tpp:the-signal", "tp:first-slot" ) )
    {
        KTERROR( testptlog, "Unable to connect tpp to tp" );
        return -1;
    }

    if( ! ptb.PushBackToRunQueue( "tpp" ) )
    {
        KTERROR( testptlog, "Unable to add tpp to the run queue" );
        return -1;
    }

    KTINFO( testptlog, "Starting asynchronous run" );
    ptb.AsyncRun();

    while( ptb.WaitForBreak() )
    {
        KTINFO( testptlog, "Reached breakpoint; continuing" );
        ptb.Continue();
    }

    KTINFO( testptlog, "Processing complete" );

    KTINFO( testptlog, "Tests complete" );

    return 0;
}
