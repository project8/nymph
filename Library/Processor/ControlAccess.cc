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
    ReturnBufferBase::ReturnBufferBase()
    {}

    ReturnBufferBase::~ReturnBufferBase()
    {}


    ControlAccess::ControlAccess() :
            fReturn( new ReturnBuffer< int >() ),
            fMutex(),
            fCondVar(),
            fBreakFlag( false ),
            fCycleTimeMS( 500 )
    {}

    ControlAccess::~ControlAccess()
    {}

    void ControlAccess::Resume()
    {
        boost::unique_lock< boost::mutex > lock( fMutex );
        fBreakFlag.store( false );
        fCondVar.notify_one();
    }

    void ControlAccess::Break()
    {
        boost::unique_lock< boost::mutex > lock( fMutex );
        fBreakFlag.store( true );
        while( fBreakFlag.load() && ! is_canceled() )
        {
            fCondVar.wait_for( lock, boost::chrono::milliseconds(fCycleTimeMS) );
        }
    }


} /* namespace Nymph */
