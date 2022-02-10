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
    try
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

        // set breakpoint, thread 1
        if( ! ptb.SetBreakpoint( "tp1", "first-slot" ) )
        {
            KTERROR( testptlog, "Unable to set the breakpoint" );
            return -1;
        }

        // no breakpoint, thread2

        // make connections, thread 1
        if( ! ptb.MakeConnection( "tpp1:the-signal", "tp1:first-slot" ) )
        {
            KTERROR( testptlog, "Unable to connect tpp1 to tp1" );
            return -1;
        }

        // make connections, thread 2
        if( ! ptb.MakeConnection( "tpp2:the-signal", "tp2:second-slot" ) )
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

        KTINFO( testptlog, "Starting asynchronous breakpoint user" );
        auto buFuture = boost::async( boost::launch::async, [&](){
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
    }
    catch( boost::exception& e )
    {
        KTERROR( testptlog, "Exception caught: " << diagnostic_information( e ) );
        return -1;
    }
    return 0;
}
