/*
 * TestController.cc
 *
 *  Created on: Jan 21, 2022
 *      Author: N.S. Oblath
 */


#include "Controller.hh"

#include "Exception.hh"
#include "QuitChain.hh"

#include "param_codec.hh"

#include <thread>

#include "catch.hpp"

namespace NymphTesting
{
    class ControllerRevealer : public Nymph::Controller
    {
        public:
            using Nymph::Controller::Controller;
            virtual ~ControllerRevealer() {}

            void SetBreakFlag( bool flag )
            {
                fBreakFlag = flag;
                return;
            }

            void SetCanceled( bool flag )
            {
                f_canceled.store( flag );
                return;
            }

            void do_cancellation( int code )
            {
                Controller::do_cancellation( code );
                return;
            }
    };
}

// unit tests
TEST_CASE( "controller", "[control]" )
{
    using namespace Nymph;
    using namespace NymphTesting;

    ControllerRevealer tcControl;

    SECTION( "Basics" )
    {
        tcControl.SetCycleTimeMS( 100 );
        REQUIRE( tcControl.GetCycleTimeMS() == 100 );
        tcControl.SetCycleTimeMS( 500 );

        REQUIRE_NOTHROW( tcControl.Mutex().lock() );
        REQUIRE_NOTHROW( tcControl.Mutex().unlock() );

        REQUIRE_FALSE( tcControl.GetBreakFlag() );

        REQUIRE_FALSE( tcControl.IsCanceled() );
        tcControl.Cancel( 5 );
        REQUIRE( tcControl.IsCanceled() );
    }

    SECTION( "Configure" )
    {
        std::string config_str(
            "cycle-time-ms: 10\n"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( tcControl.Configure( config->as_node() ) );
        REQUIRE( tcControl.GetCycleTimeMS() == 10 );
    }

    SECTION( "WaitToContinue" )
    {
        tcControl.SetCanceled( false );
        tcControl.SetBreakFlag( false );
        REQUIRE( tcControl.WaitToContinue() );

        tcControl.SetBreakFlag( true );
        tcControl.SetCanceled( true );
        REQUIRE_FALSE( tcControl.WaitToContinue() );    
    }

    SECTION( "WaitForBreakOrCanceled" )
    {
        tcControl.SetCanceled( false );
        tcControl.SetBreakFlag( true );
        REQUIRE( tcControl.WaitForBreakOrCanceled() == tcControl.GetBreakFlag() );

        tcControl.SetCanceled( true );
        tcControl.SetBreakFlag( false );
        REQUIRE_FALSE( tcControl.WaitForBreakOrCanceled() );


    }

    SECTION( "BreakContinue" )
    {
        REQUIRE_FALSE( tcControl.GetBreakFlag() );
        REQUIRE_FALSE( tcControl.IsCanceled() );

        // This thread will do the breaking and continuing
        std::thread thread( [&](){ 
            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            tcControl.Break(); // step 1

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            tcControl.Continue(); // step 2

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            tcControl.Cancel( 0 ); // step 3
        } );
        
        // Waiting for break (step 1)
        REQUIRE( tcControl.WaitForBreakOrCanceled() );
        REQUIRE( tcControl.GetBreakFlag() );
        REQUIRE_FALSE( tcControl.IsCanceled() );

        // Waiting for continue (step 2)
        tcControl.WaitToContinue();
        REQUIRE_FALSE( tcControl.GetBreakFlag() );
        REQUIRE_FALSE( tcControl.IsCanceled() );

        // Waiting for cancelation (step 3)
        REQUIRE_FALSE( tcControl.WaitForBreakOrCanceled() );
        REQUIRE_FALSE( tcControl.GetBreakFlag() );
        REQUIRE( tcControl.IsCanceled() );

        thread.join();

    }

    SECTION( "ChainIsQuitting" )
    {
        // As if an error is thrown
        std::exception_ptr exceptPtr = std::make_exception_ptr( Exception() << "Test: ChainIsQuitting, throwing Exception" );
        REQUIRE_NOTHROW( tcControl.ChainIsQuitting( "TestController::ChainIsQuitting::Exception", exceptPtr ) );

        tcControl.reset_cancel();

        // As if QuitChain is thrown
        std::exception_ptr qcPtr = std::make_exception_ptr( QuitChain() << "Test: ChainIsQuitting, throwing QuitChain" );
        REQUIRE_NOTHROW( tcControl.ChainIsQuitting( "TestController::ChainIsQuitting::QuitChain", qcPtr ) );

        tcControl.reset_cancel();

        // As if std::runtime_error is thrown; not handled by ChainIsQuitting()
        std::exception_ptr runtimePtr = std::make_exception_ptr( std::runtime_error("Test: ChainIsQuitting, throwing std::runtime_error") );
        REQUIRE_THROWS_AS( tcControl.ChainIsQuitting( "TestController::ChainIsQuitting::QuitChain", runtimePtr ), std::runtime_error );

    }

    SECTION( "ReturnBuffer" )
    {
        REQUIRE_FALSE( tcControl.HasReturn() );
        REQUIRE_THROWS_AS( tcControl.GetReturn<double>(), Exception ); // have to pick a return argument, so I just picked <double> in this case; it could be anything for this test
        double retval = 5;
        auto retBuf = tcControl.BreakAndReturn(retval);
        // check that we're now at a break point
        REQUIRE( tcControl.IsAtBreak() );
        REQUIRE( tcControl.HasReturn() );
        // we can access the return variable through the buffer
        REQUIRE( std::get<0>( retBuf ) == Approx(5.) );
        // we can change the value of the return variable using the buffer
        std::get<0>( retBuf ) = 10.;
        REQUIRE( retval == Approx(10.) );
    }

}
