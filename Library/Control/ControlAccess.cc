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

    }

    ControlAccess::~ControlAccess()
    {}

    bool ControlAccess::WaitToContinue()
    {
        if( fControl ) return fControl->WaitToContinue();
        THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
    }

    bool ControlAccess::WaitForBreakOrCanceled()
    {
        if( fControl ) return fControl->WaitForBreakOrCanceled();
        THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
    }

    void ControlAccess::WaitForEndOfRun()
    {
        if( fControl ) fControl->WaitForEndOfRun();
        else THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
        return;
    }

    void ControlAccess::Continue()
    {
        if( fControl ) fControl->Continue();
        else THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
        return;
    }

    void ControlAccess::Cancel( int code )
    {
        if( fControl ) fControl->Cancel( code );
        else THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
        return;
    }

    bool ControlAccess::IsCanceled() const
    {
        if( fControl ) return fControl->IsCanceled();
        THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
    }

    void ControlAccess::Break()
    {
        if( fControl ) fControl->Break();
        else THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
        return;
    }

    bool ControlAccess::IsAtBreak() const
    {
        if( fControl ) return fControl->IsAtBreak();
        THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
    }

    void ControlAccess::ChainIsQuitting( const std::string& name, std::exception_ptr ePtr )
    {
        if( fControl ) fControl->ChainIsQuitting( name, ePtr );
        else THROW_EXCEPT_HERE( Exception() << "Control access does not have a valid controller pointer" );
        return;
    }



    ReturnAccess::ReturnAccess() :
            fReturn( new ReturnBuffer< int >() )//,
            //fControl()
    {}

    ReturnAccess::~ReturnAccess()
    {}

} /* namespace Nymph */
