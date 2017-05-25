/*
 * TestUseBreakpoint.cc
 *
 *  Created on: May 5, 2017
 *      Author: N.S. Oblath
 */

#include "KTProcessorToolbox.hh"

#include "KTLogger.hh"


using namespace Nymph;

KTLOGGER( testptlog, "TestUseBreakpoint" )

int main()
{
    try
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
        if( ! ptb.SetBreakpoint( "tp", "first-slot" ) )
        {
            KTERROR( testptlog, "Unable to set the breakpoint" );
            return -1;
        }

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

        ptb.JoinRunThread();

        KTINFO( testptlog, "Tests complete" );
    }
    catch( std::exception& e )
    {
        KTERROR( testptlog, "Caught exception: " << e.what() );
    }
    catch( boost::exception& e )
    {
        KTERROR( testptlog, "Caught exception: " << diagnostic_information( e ) );
    }

    return 0;
}
