/*
 * TestPrimaryProcessor.cc
 *
 *  Created on: May 4, 2017
 *      Author: N.S. Oblath
 */

#include "KTTestPrimaryProcessor.hh"
#include "KTTestProcessor.hh"

#include "KTLogger.hh"

#include <thread>

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
    KTThreadReference exeThreadRef;
    auto exeThreadFuture = exeThreadRef.fDataPtrRet.get_future();

    // run the thread
    std::thread thread( &KTPrimaryProcessor::operator(), &tpp, std::move( exeThreadRef ) );

    // wait for a result to be set
    exeThreadFuture.wait();

    try
    {
        exeThreadFuture.get();
    }
    catch( std::exception& e )
    {
        exeThreadRef.fThreadIndicator->fCanceled = true;
        KTERROR( testpplog, "An error occurred while running a processor: " << e.what() );
    }

    thread.join();

    KTINFO( testpplog, "Tests complete" );

    return 0;
}
