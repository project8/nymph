/*
 * Controller.cc
 *
 *  Created on: Jan 13, 2022
 *      Author: N.S. Oblath
 */

#include "Controller.hh"

#include "ControlAccess.hh"
#include "Exception.hh"
#include "QuitChain.hh"

#include "logger.hh"


namespace Nymph
{
    LOGGER( contlog, "Controller" );

    Controller::Controller() :
            ControllerInterface(),
            scarab::cancelable(),
            fCycleTimeMS( 500 ),
            fMutex(),
            fCondVarContinue(),
            fCondVarBreak(),
            fBreakFlag( false )
    {
        ControlAccess::get_instance()->SetControl( this );
    }

    Controller::~Controller()
    {}

    void Controller::Configure( const scarab::param_node& node )
    {
        SetCycleTimeMS( node.get_value( "cycle-time-ms", fCycleTimeMS ) );

        return;
    }

    bool Controller::WaitToContinue()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        while( fBreakFlag && ! is_canceled() )
        {
            fCondVarContinue.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
        }
        return ! is_canceled(); // returns true if the thread should continue; false if it should end
    }

    bool Controller::WaitForBreakOrCanceled()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        while( ! fBreakFlag && ! is_canceled() )
        {
            fCondVarBreak.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
        }
        return is_canceled() ? false : fBreakFlag; // returns true if the thread should continue; false if it should end
    }

    void Controller::WaitForEndOfRun()
    {
        LDEBUG( contlog, "Waiting for end-of-run" );
        while( WaitForBreakOrCanceled() )
        {
            LDEBUG( contlog, "Reached breakpoint; waiting for continue" );
            if( ! WaitToContinue() ) break;
            LDEBUG( contlog, "Processing resuming; waiting for end-of-run" );
        }
        LDEBUG( contlog, "End-of-run reached" );

        return;
    }

    void Controller::Continue()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "RESUME called" );
        fBreakFlag = false;
        fCondVarContinue.notify_all();
        return;
    }

    void Controller::Cancel( int code )
    {
        this->cancel( code );
        return;
    }

    bool Controller::IsCanceled() const
    {
        return this->is_canceled();
    }

    void Controller::Break()
    {
        while( IsAtBreak() && ! IsCanceled() )
        {
            if( ! WaitToContinue() )
            {
                THROW_EXCEPT_HERE( Exception() << "Canceled while waiting to initiate a breakpoint" );
            }
        }

        std::unique_lock< std::mutex > lock( fMutex );
        fBreakFlag = true;
        fCondVarBreak.notify_all();
        return;
    }

    bool Controller::IsAtBreak() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        return fBreakFlag;
    }

    void Controller::ChainIsQuitting( const std::string& name, std::exception_ptr ePtr )
    {
        LDEBUG( contlog, "Chain <" << name << "> is quitting" );

        if( ePtr )
        {
            try
            {
                std::rethrow_exception( ePtr );
            }
            catch( const QuitChain& e )
            {
                // not necessarily an error, so don't set quitAfterThis to true
                LINFO( contlog, "Chain <" << name << "> exited with QuitChain" );
            }
            catch( const scarab::base_exception& e )
            {
                // this is an error, so set quitAfterThis to true
                LERROR( contlog, "Chain <" << name << "> exited with an exception" );
                PrintException( e );
                this->Cancel( RETURN_ERROR );
            }
        }

        return;
    }

    void Controller::do_cancellation( int code )
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "CANCEL called" );
        fCondVarBreak.notify_all();
        return;
    }

} /* namespace Nymph */
