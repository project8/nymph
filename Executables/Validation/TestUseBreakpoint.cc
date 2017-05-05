/*
 * TestUseBreakpoint.cc
 *
 *  Created on: May 5, 2017
 *      Author: N.S. Oblath
 */

#include "KTProcessorToolbox.hh"

#include "KTProcessor.hh" // temporary (todo below)

#include "KTLogger.hh"

using namespace Nymph;

KTLOGGER( testptlog, "TestUseBreakpoint" )

int main()
{
    KTINFO( testptlog, "Preparing to run" );

    KTProcessorToolbox ptb;

    // add the primary processor
    if( ! ptb.AddProcessor( "test-p-proc", "tpp" ) )
    {
        KTERROR( testptlog, "Unable to create test primary processor" );
        return -1;
    }

    // add processor b
    if( ! ptb.AddProcessor( "test-proc-b", "tp" ) )
    {
        KTERROR( testptlog, "Unable to create test processor b" );
        return -1;
    }
    // set breakpoint
    // TODO: use configuration instead of manually setting
    ptb.GetProcessor( "tp" )->SetDoBreakpoint( "first-slot", true );

    // make connections
    if( ! ptb.MakeConnection( "tpp:the-signal", "tp:first-slot" ) )
    {
        KTERROR( testptlog, "Unable to connect tpp to tp" );
        return -1;
    }

    // set the run queue
    if( ! ptb.PushBackToRunQueue( "tpp" ) )
    {
        KTERROR( testptlog, "Unable to add tpp to the run queue" );
        return -1;
    }

    // go!
    KTINFO( testptlog, "Starting asynchronous run" );
    ptb.AsyncRun();

    KTINFO( testptlog, "Starting asynchronous breakpoint user" );
    auto buFuture = std::async( std::launch::async, [&](){
        KTINFO( testptlog, "In breakpoint user thread" );
        while( ptb.WaitForBreak() )
        {
            KTPROG( testptlog, "Reached breakpoint; waiting for user input" );
            std::string temp;
            KTPROG( testptlog, "Please press [return]" );
            getline( std::cin, temp );
            ptb.Continue();
        }
        KTINFO( testptlog, "Finished breakpoint user thread" );
        return;
    });

    KTINFO( testptlog, "Waiting for the end of the run" );
    ptb.WaitForEndOfRun();

    KTINFO( testptlog, "Processing complete" );

    KTINFO( testptlog, "Tests complete" );

    return 0;
}
