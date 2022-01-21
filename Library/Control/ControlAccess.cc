/*
 * ControlAccess.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "ControlAccess.hh"

#include "logger.hh"

LOGGER( contlog, "ControlAccess" );

namespace Nymph
{
    ControlAccess::ControlAccess() :
            scarab::singleton< ControlAccess >(),
            ControllerInterface(),
            fControl( nullptr )
    {
        fControl = Controller::
    }

    ControlAccess::~ControlAccess()
    {}

    bool ControlAccess::WaitToContinue()
    {
        return fControl->WaitToContinue();
    }

    bool ControlAccess::WaitForBreakOrCanceled()
    {
        return fControl->WaitForBreakOrCanceled();
    }

    void ControlAccess::WaitForEndOfRun()
    {
        fControl->WaitForEndOfRun();
        return;
    }

    void ControlAccess::Continue()
    {
        fControl->Continue();
        return;
    }

    void ControlAccess::Cancel( int code )
    {
        fControl->Cancel( code );
        return;
    }

    bool ControlAccess::IsCanceled() const
    {
        return fControl->IsCanceled();
    }

    void ControlAccess::Break()
    {
        fControl->Break();
        return;
    }

    bool ControlAccess::IsAtBreak() const
    {
        return fControl->IsAtBreak();
    }

    void ControlAccess::ChainQuitting( std::exception_ptr ePtr )
    {
        fControl->ChainQuitting( ePtr );
        return;
    }



    ReturnAccess::ReturnAccess() :
            fReturn( new ReturnBuffer< int >() )//,
            //fControl()
    {}

    ReturnAccess::~ReturnAccess()
    {}

} /* namespace Nymph */
