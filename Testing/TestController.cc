/*
 * TestController.cc
 *
 *  Created on: Jan 21, 2022
 *      Author: N.S. Oblath
 */


#include "Controller.hh"

#include "Exception.hh"
#include "QuitChain.hh"

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
        REQUIRE_NOTHROW( control.Mutex().lock() );
        REQUIRE_NOTHROW( control.Mutex().unlock() );

        REQUIRE_FALSE( control.GetBreakFlag() );

        control.SetCycleTimeMS( 100 );
        REQUIRE( control.GetCycleTimeMS() == 100 );
        control.SetCycleTimeMS( 500 );

        REQUIRE_FALSE( control.IsCanceled() );
        control.Cancel( 5 );
        REQUIRE( control.IsCanceled() );
    }

    SECTION( "WaitToContinue" )
    {
        control.SetCanceled( false );
        control.SetBreakFlag( false );
        REQUIRE( control.WaitToContinue() );

        control.SetBreakFlag( true );
        control.SetCanceled( true );
        REQUIRE_FALSE( control.WaitToContinue() );    

        // TODO: test with fBreakFlag==true && cancelled==false requires a thread
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

/*
                virtual bool WaitToContinue();

            /// Use this to wait for a breakpoint to be reached or for cancellation
            /// If the return is true, processing can continue after the break
            /// If the return is false, processing has ended (either normally or due to an error)
            virtual bool WaitForBreakOrCanceled();

            /// Use this to have a thread wait for the end of a run
            virtual void WaitForEndOfRun();

            /// Instruct the Controller to continue after a breakpoint
            virtual void Continue();

            /// Cancel all threads and end the run
            virtual void Cancel( int code = 0 );

            /// Reports whether controls is canceled
            virtual bool IsCanceled() const;

            /// Inititate a break
            virtual void Break();

            /// Reports whether control is at a breakpoint
            virtual bool IsAtBreak() const;

            /// Notify the control that a chain is quitting
            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() );

            MEMVAR_REF_MUTABLE( std::mutex, Mutex );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarContinue );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarBreak );
            MEMVAR_NOSET( bool, BreakFlag );
            MEMVAR( unsigned, CycleTimeMS );
            //MEMVAR_SHARED_PTR_CONST( ReturnBufferBase, ReturnPtr );
*/
}

/*
TEST_CASE( "controller", "[control]" )
{
    using namespace Nymph;

    Controller control;

    SECTION( "Break" )
    {
        REQUIRE_FALSE( control.GetBreakFlag() );

        std::thread thread( [&](){ 
            control.Break();
        } );
        
        std::this_thread::sleep_for( std::chrono::milliseconds(100) );
        REQUIRE( control.GetBreakFlag() );

        control.Continue();
        std::this_thread::sleep_for( std::chrono::milliseconds(100) );
        REQUIRE_FALSE( control.GetBreakFlag() );

        thread.join();

        REQUIRE_FALSE( control.GetBreakFlag() );
    }

}
*/