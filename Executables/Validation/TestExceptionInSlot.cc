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

    try
    {
        KTTestProcessorA tpA;
        KTTestProcessorC tpC;

        KTINFO( testexclog, "Connecting the-signal to first-slot" );
        tpA.ConnectASlot( "the-signal", &tpC, "first-slot", 20 );

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
            catch( boost::exception& e )
            {
                exeThreadRef->SetReturnException( boost::current_exception() );
            }

            return;
        };

        // run the thread
        boost::thread thread( exeFunc );

        // wait for a result to be set
        exeThreadRef->GetDataPtrRetFuture().wait();

        try
        {
            exeThreadRef->GetReturnValue();
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
    }
    catch( boost::exception& e )
    {
        KTERROR( testexclog, "An unexpected exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
}
