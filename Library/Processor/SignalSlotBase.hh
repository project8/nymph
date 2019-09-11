/*
 * SignalSlotBase.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_SIGNALSLOTBASE_HH_
#define NYMPH_SIGNALSLOTBASE_HH_

#include "Exception.hh"
#include "MemberVariable.hh"

#include <memory>
#include <set>
#include <string>

namespace Nymph
{
    struct SignalException : virtual public Exception {};
    struct SlotException : virtual public Exception {};

    struct ConnectionException : public Exception {};


    class SignalBase;
    class SlotBase;

    typedef std::shared_ptr< SignalBase > SignalPtr_t;
    typedef std::shared_ptr< SlotBase > SlotPtr_t;

    class SlotBase : public std::enable_shared_from_this< SlotBase >
    {
        public:
            SlotBase( const std::string& name );
            //template< typename XOwner >
            //SlotBase( const std::string& name, XOwner* owner );
            virtual ~SlotBase();

            virtual void ConnectTo( SignalPtr_t signal, int group = -1 ) = 0;

            void Disconnect( SignalPtr_t signal);
            void DisconnectAll();

            MEMVAR_REF( std::string, Name );

            typedef std::set< SignalPtr_t > signal_connections; // to get around the problem of having a comma inside a macro function argument
            MEMVAR_REF_MUTABLE( signal_connections, Connections );

        protected:
            friend class SignalBase;
            virtual void AddConnection( SignalPtr_t signal );
    };


    class SignalBase : public std::enable_shared_from_this< SignalBase >
    {
        public:
            SignalBase( const std::string& name );
            //template< typename XOwner >
            //SignalBase( const std::string& name, XOwner* owner );
            virtual ~SignalBase();

            virtual void Connect( SlotPtr_t slot, int group = -1 ) = 0;

            void Disconnect( const SlotPtr_t slot );
            void DisconnectAll();

            MEMVAR_REF( std::string, Name );

            typedef std::set< SlotPtr_t > slot_connections; // to get around the problem of having a comma inside a macro function argument
            MEMVAR_REF_MUTABLE_CONST( slot_connections, Connections );

        protected:
            friend class SignalBase;
            virtual void AddConnection( SlotPtr_t slot, int group );
    };

    inline void SlotBase::AddConnection( SignalPtr_t signal )
    {
        fConnections.insert( signal );
        return;
    }

    inline void SlotBase::Disconnect( SignalPtr_t signal )
    {
        signal->Disconnect( shared_from_this() );
        return;
    }

    inline void SignalBase::AddConnection( SlotPtr_t slot, int group )
    {
        fConnections.insert( slot );
        slot->AddConnection( shared_from_this() );
        return;
    }

    // disconnects a previously connected function
    inline void SignalBase::Disconnect( SlotPtr_t slot )
    {
        slot->fConnections.erase( shared_from_this() );
        fConnections.erase( slot );
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_SIGNALSLOTBASE_HH_ */
