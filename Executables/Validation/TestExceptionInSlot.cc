/*
 * TestExceptionInSlot.cc
 *
 *  Created on: May 4, 2017
 *      Author: N. Oblath
 *
 *  Intended outcome: Exception is thrown in first-slot when the first signal (5) is emitted; the second signal should not be emitted.
 */

#include "KTTestProcessor.hh"
#include "KTLogger.hh"

#include <thread>
#include <atomic>

using namespace Nymph;

KTLOGGER( testexclog, "TestExceptionInSlot" )

int main()
{

    KTTestProcessorA tpA;
    KTTestProcessorC tpC;

    KTINFO( testexclog, "Connecting the-signal to first-slot" );
    try
    {
        tpA.ConnectASlot( "the-signal", &tpC, "first-slot", 20 );
    }
    catch( std::exception& e )
    {
        KTERROR( testexclog, "A problem occurred while connecting the signal and slots:\n" << e.what() );
        return -1;
    }

    // setup to execute processors asynchronously
    KTThreadReference exeThreadRef;
    auto exeThreadFuture = exeThreadRef.fDataPtrRet.get_future();
    tpC.GetSlot( "first-slot" )->SetThreadRef (&exeThreadRef );

    std::atomic< bool > canceled( false );

    auto exeFunc = [&]() {
        try
        {
            std::this_thread::sleep_for( std::chrono::milliseconds(1) ); // delay to let the main thread get to the std::future::wait() call

            KTINFO( testexclog, "Emitting signals" );

            if( canceled.load() ) return;
            KTINFO( testexclog, "First test signal: 5" );
            tpA.EmitSignals( 5 );

            std::this_thread::sleep_for( std::chrono::milliseconds(1) ); // delay to let the main thread react to the exception

            if( canceled.load() ) return;
            KTINFO( testexclog, "Second test signal: 18" );
            tpA.EmitSignals( 18 );
        }
        catch( ... )
        {
            exeThreadRef.fDataPtrRet.set_exception( std::current_exception() );
        }

        return;
    };

    // run the thread
    std::thread thread( exeFunc );

    // wait for a result to be set
    exeThreadFuture.wait();

    try
    {
        exeThreadFuture.get();
    }
    catch( std::exception& e )
    {
        canceled = true;
        KTERROR( testexclog, "An error occurred while running a processor: " << e.what() );
    }

    thread.join();

    KTINFO( testexclog, "Tests complete" );

    return 0;
}
