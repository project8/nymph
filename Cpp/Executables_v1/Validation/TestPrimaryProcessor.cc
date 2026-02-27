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

    try
    {
        KTTestPrimaryProcessor tpp;
        KTTestProcessorB tp;

        KTINFO( testpplog, "Connecting the-signal to first-slot" );
        tpp.ConnectASlot( "the-signal", &tp, "first-slot", 20 );

        // setup to execute processors asynchronously
        std::shared_ptr< KTThreadReference > exeThreadRef( std::make_shared< KTThreadReference >() );
        exeThreadRef->Name() = std::string( "tpp" );

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
        exeThreadRef->GetDataPtrRetFuture().wait();

        try
        {
            exeThreadRef->GetReturnValue();
        }
        catch( boost::exception& e )
        {
            exeThreadRef->SetCanceled( true );
            KTERROR( testpplog, "An error occurred while running a processor: " << diagnostic_information( e ) );
        }

        thread.join();

        KTINFO( testpplog, "Tests complete" );
    }
    catch( boost::exception& e )
    {
        KTERROR( testpplog, "Exception caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
}
