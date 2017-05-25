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

#include <boost/thread.hpp>
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
    std::shared_ptr< KTThreadReference > exeThreadRef = std::make_shared< KTThreadReference >();
    //boost::shared_future< KTDataPtr > exeThreadFuture = exeThreadRef->fDataPtrRetFuture;
    tpC.GetSlot( "first-slot" )->SetThreadRef( exeThreadRef );

    std::atomic< bool > canceled( false );

    auto exeFunc = [&]() {
        try
        {
            boost::this_thread::sleep_for( boost::chrono::milliseconds(1) ); // delay to let the main thread get to the std::future::wait() call

            KTINFO( testexclog, "Emitting signals" );

            if( canceled.load() ) return;
            KTINFO( testexclog, "First test signal: 5" );
            tpA.EmitSignals( 5 );

            boost::this_thread::sleep_for( boost::chrono::milliseconds(1) ); // delay to let the main thread react to the exception

            if( canceled.load() ) return;
            KTINFO( testexclog, "Second test signal: 18" );
            tpA.EmitSignals( 18 );
        }
        catch( ... )
        {
            exeThreadRef->fDataPtrRet.set_exception( boost::current_exception() );
        }

        return;
    };

    // run the thread
    boost::thread thread( exeFunc );

    // wait for a result to be set
    exeThreadRef->fDataPtrRetFuture.wait();

    try
    {
        exeThreadRef->fDataPtrRetFuture.get();
    }
    //catch( std::exception& e )
    //{
    //    canceled = true;
    //    KTERROR( testexclog, "An error occurred while running a processor: " << e.what() );
    //}
    catch( boost::exception& e )
    {
        canceled = true;
        KTERROR( testexclog, "An error occurred while running a processor: " << diagnostic_information( e ) );
    }

    thread.join();

    KTINFO( testexclog, "Tests complete" );

    return 0;
}
