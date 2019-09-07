/*
 * Signal.hh
 *
 *  Created on: Jan 15, 2013
 *      Author: N.S. Oblath
 * 
 *  Based on this implementation: https://schneegans.github.io/tutorials/2015/09/20/signal-slot
 */

#ifndef NYMPH_SIGNAL_HH_
#define NYMPH_SIGNAL_HH_

#include "SignalSlotBase.hh"

#include "Exception.hh"
#include "Logger.hh"

#include <map>

namespace Nymph
{
    LOGGER( signallog, "Signal" );

    /// A Signal object may call multiple slots with the
    /// same signature. You can connect functions to the Signal
    /// that will be called when the emit() method on the
    /// Signal object is invoked. Any argument passed to emit()
    /// will be passed to the given functions.
    template< typename... XArgs >
    class Signal : public SignalBase
    {
        public:
            using signature = void( XArgs... );

        public:
            Signal( const std::string& name );
            template< typename XOwner >
            Signal( const std::string& name, XOwner* owner );
            Signal( const Signal& ) = delete;
            Signal( Signal&& ) = delete;
            virtual ~Signal();

            virtual void Connect( SlotPtr_t slot, int group );

            // disconnects a previously connected function
            void Disconnect( SlotPtr_t slot ) const;

            // disconnects all previously connected functions
            void DisconnectAll() const;

            // calls all connected functions
            void Emit( XArgs... args );
            void operator()( XArgs... args );

            typedef std::set< SlotPtr_t > slot_connections; // to get around the problem of having a comma inside a macro function argument
            MEMVAR_REF_MUTABLE_CONST( slot_connections, Connections );

        protected:
            friend class Slot< XArgs... >;
            void _Connect( SlotPtr_t slot, int group );
    };


    /*!
     @class Signal
     @author N. S. Oblath

     @brief Creates a signal that takes 0 or more arguments.

     @details
     The signal is emitted by calling operator().
     If a Slot is being used, and the Slot has been given a pointer to this signal, the Slot will emit the Signal.

     Usage:
     In your Processor's header add a member variable of type Signal< ArgumentTypes >.

     Initialize the signal with the processor's 'this' pointer and the name of the signal.

     To use the signal, call it as: fSignalObject(arg);
    *//*
    template< class... XSignalArguments >
    class Signal
    {
        public:
            typedef void (signature)( XSignalArguments... );
            typedef boost::signals2::signal< signature > boost_signal;
            typedef typename boost::signals2::signal< signature >::slot_type slot_type;

        public:
            Signal();
            Signal( const std::string& name, Processor* proc );
            virtual ~Signal();

        protected:
            Signal( const Signal& );

        public:
            void operator()( XSignalArguments... args );

            boost_signal* Signal();

            const std::string& GetName() const;

        protected:
            boost_signal fSignal;

            std::string fName;
    };
*/


    //*******************
    // Implementations
    //*******************

    template< typename... XArgs >
    Signal< XArgs... >::Signal( const std::string& name ) :
            SignalBase( name ),
            fConnections()
    {}

    template< typename... XArgs >
    template< typename XOwner >
    Signal< XArgs... >::Signal< XOwner >( const std::string& name, XOwner* owner ) :
            fSlots()
    {
        owner->RegisterSignal( name, this );
    }

    template< typename... XArgs >
    Signal< XArgs... >::~Signal()
    {
        DisconnectAll();
    }

    template< typename... XArgs >
    void Signal< XArgs... >::Connect( SlotPtr_t slot, int group )
    {
        if( fSlots.count( slot ) != 0 )
        {
            LWARN( signallog, "Signal <" << fName << "> is already connected to slot <" << slot->Name() << ">" );
            return;
        }

        // ensure that the slot is of the correct type
        if( ! std::dynamic_pointer_cast< Slot< XArgs... > >( slot ) )
        {
            BOOST_THROW_EXCEPTION( ConnectionException() << "Trying to connect signal <" << fName << "> to slot <" << slot->Name() << ">, but cannot make the connection:\n" <<
                    "\tUnable to cast from SlotBase to this signal's derived type.\n" << 
                    "\tArgument types do not match" << eom );
        }
/*
        Connection connection;
        if( group >= 0 )
        {
            connection = fInternalSignal.connect( group, derivedSlot->Function() );
        }
        else
        {
            connection = fInternalSignal.connect( derivedSlot->Function() );
        }
*/
        _Connect( slot, group );
        slot->_AddConnection( this );

        return;
    }

    template< typename... XArgs >
    void Signal< XArgs... >::_Connect( SlotPtr_t slot, int group )
    {
        fConnections.insert( slot );
        return;
    }

    // disconnects a previously connected function
    template< typename... XArgs >
    void Signal< XArgs... >::Disconnect( SlotPtr_t slot ) const
    {
        slot->RemoveConnection( this );
        return;
    }

    // disconnects all previously connected functions
    template< typename... XArgs >
    void Signal< XArgs... >::DisconnectAll() const
    {
        for( auto connection : fSlots )
        {
            connection->RemoveConnection( this );
        }
        return;
    }

    // calls all connected functions
    template< typename... XArgs >
    inline void Signal< XArgs... >::Emit( XArgs... args )
    {
        (*this)( args );
        return;
    }

    template< typename... XArgs >
    inline void Signal< XArgs... >::operator()( XArgs... args )
    {
        for( auto connection : fSlots )
        {
            connection->Function( args );
        }
        return;
    }

/*
    template< class... XSignalArguments >
    Signal< XSignalArguments... >::Signal( const std::string& name, Processor* proc ) :
            fSignal(),
            fName( name )
    {
        proc->RegisterSignal(name, &fSignal);
    }

    template< class... XSignalArguments >
    Signal< XSignalArguments... >::Signal() :
            fSignal(),
            fName("none")
    {}

    template< class... XSignalArguments >
    Signal< XSignalArguments... >::Signal( const Signal& signal ) :
            fSignal(),
            fName( signal.fName )
    {}

    template< class... XSignalArguments >
    Signal< XSignalArguments... >::~Signal()
    {
    }

    template< class... XSignalArguments >
    inline void Signal< XSignalArguments... >::operator()( XSignalArguments... args )
    {
        fSignal( args... );
    }

    template< class... XSignalArguments >
    inline typename Signal< XSignalArguments... >::boost_signal* Signal< XSignalArguments... >::Signal()
    {
        return &fSignal;
    }

    template< class... XSignalArguments >
    inline const std::string& Signal< XSignalArguments... >::GetName() const
    {
        return fName;
    }
*/
} /* namespace Nymph */

#endif /* NYMPH_SIGNAL_HH_ */
