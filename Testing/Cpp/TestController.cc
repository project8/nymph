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

namespace Nymph
{
    class ControllerRevealer : public Controller
    {
        public:
            using Controller::Controller;
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

    ControllerRevealer control;

    SECTION( "Basics" )
    {
        control.SetCycleTimeMS( 100 );
        REQUIRE( control.GetCycleTimeMS() == 100 );
        control.SetCycleTimeMS( 500 );

        REQUIRE_NOTHROW( control.Mutex().lock() );
        REQUIRE_NOTHROW( control.Mutex().unlock() );

        REQUIRE_FALSE( control.GetBreakFlag() );

        REQUIRE_FALSE( control.IsCanceled() );
        control.Cancel( 5 );
        REQUIRE( control.IsCanceled() );
    }

    SECTION( "Configure" )
    {
        std::string config_str(
            "cycle-time-ms: 10\n"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

        REQUIRE_NOTHROW( control.Configure( config->as_node() ) );
        REQUIRE( control.GetCycleTimeMS() == 10 );
    }

    SECTION( "WaitToContinue" )
    {
        control.SetCanceled( false );
        control.SetBreakFlag( false );
        REQUIRE( control.WaitToContinue() );

        control.SetBreakFlag( true );
        control.SetCanceled( true );
        REQUIRE_FALSE( control.WaitToContinue() );    
    }

    SECTION( "WaitForBreakOrCanceled" )
    {
        control.SetCanceled( false );
        control.SetBreakFlag( true );
        REQUIRE( control.WaitForBreakOrCanceled() == control.GetBreakFlag() );

        control.SetCanceled( true );
        control.SetBreakFlag( false );
        REQUIRE_FALSE( control.WaitForBreakOrCanceled() );


    }

    SECTION( "BreakContinue" )
    {
        REQUIRE_FALSE( control.GetBreakFlag() );
        REQUIRE_FALSE( control.IsCanceled() );

        // This thread will do the breaking and continuing
        std::thread thread( [&](){ 
            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            control.Break(); // step 1

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            control.Continue(); // step 2

            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

            control.Cancel( 0 ); // step 3
        } );
        
        // Waiting for break (step 1)
        REQUIRE( control.WaitForBreakOrCanceled() );
        REQUIRE( control.GetBreakFlag() );
        REQUIRE_FALSE( control.IsCanceled() );

        // Waiting for continue (step 2)
        control.WaitToContinue();
        REQUIRE_FALSE( control.GetBreakFlag() );
        REQUIRE_FALSE( control.IsCanceled() );

        // Waiting for cancelation (step 3)
        REQUIRE_FALSE( control.WaitForBreakOrCanceled() );
        REQUIRE_FALSE( control.GetBreakFlag() );
        REQUIRE( control.IsCanceled() );

        thread.join();

    }

    SECTION( "ChainIsQuitting" )
    {
        // As if an error is thrown
        std::exception_ptr exceptPtr = std::make_exception_ptr( Exception() << "Test: ChainIsQuitting, throwing Exception" );
        REQUIRE_NOTHROW( control.ChainIsQuitting( "TestController::ChainIsQuitting::Exception", exceptPtr ) );

        control.reset_cancel();

        // As if QuitChain is thrown
        std::exception_ptr qcPtr = std::make_exception_ptr( QuitChain() << "Test: ChainIsQuitting, throwing QuitChain" );
        REQUIRE_NOTHROW( control.ChainIsQuitting( "TestController::ChainIsQuitting::QuitChain", qcPtr ) );

        control.reset_cancel();

        // As if std::runtime_error is thrown; not handled by ChainIsQuitting()
        std::exception_ptr runtimePtr = std::make_exception_ptr( std::runtime_error("Test: ChainIsQuitting, throwing std::runtime_error") );
        REQUIRE_THROWS_AS( control.ChainIsQuitting( "TestController::ChainIsQuitting::QuitChain", runtimePtr ), std::runtime_error );

    }

}
