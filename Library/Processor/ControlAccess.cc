/*
 * ControlAccess.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "ControlAccess.hh"

#include "logger.hh"

//#include <boost/thread/locks.hpp>
//#include <boost/thread/lock_types.hpp>

LOGGER( contlog, "ControlAccess" );

namespace Nymph
{
    SharedControl::SharedControl() :
            fMutex(),
            fCondVarContinue(),
            fCondVarBreak(),
            fBreakFlag( false ),
            fCanceledFlag( false ),
            fCycleTimeMS( 500 ),
            fNActiveThreads( 0 )
    {}

    SharedControl::~SharedControl()
    {}

    void SharedControl::Reset()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "Reseting ControlAccess" );
        fBreakFlag = false;
        fCanceledFlag = false;
        fCycleTimeMS = 500;
        fNActiveThreads = 0;
        return;
    }

    void SharedControl::Cancel()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "CANCEL called" );
        fCanceledFlag = true;
        fCondVarBreak.notify_all();
        return;
    }

    bool SharedControl::IsAtBreak() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        return fBreakFlag;
    }

    bool SharedControl::IsCanceled() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        return fCanceledFlag;
    }

    bool SharedControl::WaitToContinue() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        if( fBreakFlag )
        {
            while( fBreakFlag && ! fCanceledFlag && fNActiveThreads > 0 )
            {
                fCondVarContinue.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
            }
        }
        return (! fCanceledFlag) && (fNActiveThreads > 0); // returns true if the thread should continue; false if it should end
    }

    bool SharedControl::WaitForBreakOrCanceled() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        while( ! fBreakFlag && ! fCanceledFlag && fNActiveThreads > 0 )
        {
            fCondVarBreak.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
        }
        return fCanceledFlag || fNActiveThreads == 0 ? false : fBreakFlag;
    }

    void SharedControl::Resume()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "RESUME called" );
        fBreakFlag = false;
        fCondVarContinue.notify_all();
    }

    void SharedControl::IncrementActiveThreads()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        ++fNActiveThreads;
        LDEBUG( contlog, "Incremented active threads: " << fNActiveThreads );
        return;
    }

    void SharedControl::DecrementActiveThreads()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        if( fNActiveThreads > 0 ) --fNActiveThreads;
        LDEBUG( contlog, "Decremented active threads: " << fNActiveThreads );
        return;
    }




    ControlAccess::ControlAccess() :
            fReturn( new ReturnBuffer< int >() )//,
            //fControl()
    {}

    ControlAccess::~ControlAccess()
    {}

} /* namespace Nymph */
