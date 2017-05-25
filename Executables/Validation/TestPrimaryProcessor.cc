/*
 * TestPrimaryProcessor.cc
 *
 *  Created on: May 4, 2017
 *      Author: N.S. Oblath
 */

#include "KTTestPrimaryProcessor.hh"
#include "KTTestProcessor.hh"

#include "KTLogger.hh"

#include <boost/thread.hpp>

using namespace Nymph;

KTLOGGER( testpplog, "TestPrimaryProcessor" )

int main()
{

    KTTestPrimaryProcessor tpp;
    KTTestProcessorB tp;

    KTINFO( testpplog, "Connecting the-signal to first-slot" );
    try
    {
        tpp.ConnectASlot( "the-signal", &tp, "first-slot", 20 );
    }
    catch( std::exception& e )
    {
        KTERROR( testpplog, "A problem occurred while connecting the signal and slots:\n" << e.what() );
        return -1;
    }

    // setup to execute processors asynchronously
    //KTThreadReference exeThreadRef;
    //auto exeThreadFuture = exeThreadRef.fDataPtrRet.get_future();
    std::shared_ptr< KTThreadReference > exeThreadRef = std::make_shared< KTThreadReference >();
    exeThreadRef->fName = std::string( "tpp" );

    // run the thread
    boost::condition_variable threadStartedCV;
    boost::mutex threadStartedMutex;
    bool threadStartedFlag = false;

    boost::unique_lock< boost::mutex > threadStartedLock( threadStartedMutex );
    boost::thread thread( [&](){ tpp( exeThreadRef, threadStartedCV, threadStartedFlag ); } );

    while( ! threadStartedFlag )
    {
        threadStartedCV.wait( threadStartedLock );
    }
    KTDEBUG( testpplog, "Thread has started" );

    // wait for a result to be set
    exeThreadRef->fDataPtrRetFuture.wait();

    try
    {
        exeThreadRef->fDataPtrRetFuture.get();
    }
    catch( std::exception& e )
    {
        exeThreadRef->fCanceled = true;
        KTERROR( testpplog, "An error occurred while running a processor: " << e.what() );
    }
    catch( boost::exception& e )
    {
        exeThreadRef->fCanceled = true;
        KTERROR( testpplog, "An error occurred while running a processor: " << diagnostic_information( e ) );
    }

    thread.join();

    KTINFO( testpplog, "Tests complete" );

    return 0;
}
