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
            fConnections()//,
            //fSignalsUsed()
    {}

    SlotBase::~SlotBase()
    {}

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
            //fControl( nullptr ),
            fDoBreakpoint( false )
    {}

    SignalBase::~SignalBase()
    {}

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
