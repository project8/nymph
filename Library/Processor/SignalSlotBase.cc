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
    {}

    void SlotBase::DisconnectAll()
    {
        for( auto connection : fConnections )
        {
            connection->Disconnect( shared_from_this() );
        }
        return;
    }

    SignalBase::SignalBase( const std::string& name ) :
            fName( name ),
            fConnections()
    {}

    SignalBase::~SignalBase()
    {}

    // disconnects all previously connected functions
    void SignalBase::DisconnectAll()
    {
        for( auto connection : fConnections )
        {
            Disconnect( connection );
        }
        return;
    }


}

