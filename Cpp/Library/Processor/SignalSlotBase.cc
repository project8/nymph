/*
 * SignalSlotBase.cc
 *
 *  Created on: Sept 7, 2019
 *      Author: N.S. Oblath
 */

#include "SignalSlotBase.hh"

namespace Nymph
{
    SlotBase::SlotBase( const std::string& name ) :
            fName( name ),
            fConnections()
    {}

    SlotBase::~SlotBase()
    {
        DisconnectAll();
    }

    void SlotBase::DisconnectAll()
    {
        while( ! fConnections.empty() )
        {
            Disconnect( *fConnections.begin() );
        }
        return;
    }

    SignalBase::SignalBase( const std::string& name ) :
            fName( name ),
            fConnections(),
            fDoBreakpoint( false )
    {}

    SignalBase::~SignalBase()
    {
        DisconnectAll();
    }

    // disconnects all previously connected functions
    void SignalBase::DisconnectAll()
    {
        while( ! fConnections.empty() )
        {
            Disconnect( *fConnections.begin() );
        }
        return;
    }

}

