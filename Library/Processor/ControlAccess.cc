/*
 * ControlAccess.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "ControlAccess.hh"

//#include <boost/thread/locks.hpp>
//#include <boost/thread/lock_types.hpp>


namespace Nymph
{
    SharedControl::SharedControl() :
            fMutex(),
            fCondVarContinue(),
            fCondVarBreak(),
            fBreakFlag( false ),
            fCanceledFlag( false ),
            fCycleTimeMS( 500 )
    {}

    SharedControl::~SharedControl()
    {}

    void SharedControl::Cancel()
    {
        std::unique_lock< std::mutex > lock( fMutex );
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
            while( fBreakFlag && ! fCanceledFlag )
            {
                fCondVarContinue.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
            }
        }
        return ! fCanceledFlag; // returns true if the thread should continue; false if it should end
    }

    bool SharedControl::WaitForBreakOrCanceled() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        while( ! fBreakFlag && ! fCanceledFlag )
        {
            fCondVarBreak.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
        }
        return fCanceledFlag ? false : fBreakFlag;
    }

    void SharedControl::Resume()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        fBreakFlag = false;
        fCondVarContinue.notify_all();
    }




    ControlAccess::ControlAccess() :
            fReturn( new ReturnBuffer< int >() )//,
            //fControl()
    {}

    ControlAccess::~ControlAccess()
    {}

} /* namespace Nymph */
