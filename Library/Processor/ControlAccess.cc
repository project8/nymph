/*
 * ControlAccess.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "ControlAccess.hh"

#include <boost/thread/locks.hpp>
#include <boost/thread/lock_types.hpp>


namespace Nymph
{
    SharedControl::SharedControl() :
            fMutex(),
            fCondVar(),
            fBreakFlag( false ),
            fCanceledFlag( false ),
            fCycleTimeMS( 500 )
    {}

    SharedContrl::~SharedControl()
    {}

    void SharedContrl::Break()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        fBreakFlag = true;
        return;
    }

    void SharedContrl::Cancel()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        fCanceledFlag = true;
        return;
    }

    bool SharedContrl::IsAtBreak() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        return fBreakFlag;
    }

    bool SharedContrl::IsCanceled() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        return fCanceledFlag;
    }

    bool SharedContrl::WaitToContinue() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        if( fBreakFlag )
        {
            while( fBreakFlag && ! fCanceledFlag )
            {
                fCondVar.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
            }
        }
        return ! fCanceled; // returns true if the thread should continue; false if it should end
    }

    void SharedContrl::Resume()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        fBreakFlag = false;
        fCondVar.notify_all();
    }



    ReturnBufferBase::ReturnBufferBase()
    {}

    ReturnBufferBase::~ReturnBufferBase()
    {}


    ControlAccess::ControlAccess() :
            fReturn( new ReturnBuffer< int >() ),
            fControl()
    {}

    ControlAccess::~ControlAccess()
    {}

} /* namespace Nymph */
