/*
 * SignalSlotBase.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_SIGNALSLOTBASE_HH_
#define NYMPH_SIGNALSLOTBASE_HH_

#include "ControlAccess.hh"
#include "Exception.hh"
#include "MemberVariable.hh"

#include <memory>
#include <set>
#include <string>
#include <vector>

namespace Nymph
{
    struct SignalException : virtual public Exception {};
    struct SlotException : virtual public Exception {};

    struct ConnectionException : public Exception {};


    class SignalBase;

    class SlotBase
    {
        public:
            SlotBase( const std::string& name );
            //template< typename XOwner >
            //SlotBase( const std::string& name, XOwner* owner );
            virtual ~SlotBase();

            virtual void ConnectTo( SignalBase* signal, int group = -1 ) = 0;

            void Disconnect( SignalBase* signal);
            void DisconnectAll();

            MEMVAR_REF( std::string, Name );

            typedef std::set< SignalBase* > signal_connections; // to get around the problem of having a comma inside a macro function argument
            MEMVAR_REF_MUTABLE( signal_connections, Connections );

            MEMVAR_REF( std::vector< SignalBase* >, SignalsUsed );

        protected:
            friend class SignalBase;
            virtual void AddConnection( SignalBase* signal );
    };


    class SignalBase
    {
        public:
            SignalBase( const std::string& name );
            //template< typename XOwner >
            //SignalBase( const std::string& name, XOwner* owner );
            virtual ~SignalBase();

            virtual void Connect( SlotBase* slot, int group = -1 ) = 0;

            void Disconnect( SlotBase* slot );
            void DisconnectAll();

            MEMVAR_REF( std::string, Name );

            typedef std::set< SlotBase* > slot_connections; // to get around the problem of having a comma inside a macro function argument
            MEMVAR_REF_MUTABLE_CONST( slot_connections, Connections );

            MEMVAR( ControlAccess*, Control ); // doesn't use MEMVAR_PTR because Signal doesn't own the object pointed to by fControl

            MEMVAR( bool, DoBreakpoint );

        protected:
            friend class SignalBase;
            virtual void AddConnection( SlotBase* slot, int group );
    };

    inline void SlotBase::AddConnection( SignalBase* signal )
    {
        fConnections.insert( signal );
        return;
    }

    inline void SlotBase::Disconnect( SignalBase* signal )
    {
        signal->Disconnect( this );
        return;
    }

    inline void SignalBase::AddConnection( SlotBase* slot, int group )
    {
        fConnections.insert( slot );
        slot->AddConnection( this );
        return;
    }

    // disconnects a previously connected function
    inline void SignalBase::Disconnect( SlotBase* slot )
    {
        slot->fConnections.erase( this );
        fConnections.erase( slot );
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_SIGNALSLOTBASE_HH_ */
