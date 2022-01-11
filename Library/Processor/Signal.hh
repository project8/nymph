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

#include "logger.hh"


namespace Nymph
{
    LOGGER( signallog, "Signal" );

    template< typename... XArgs >
    class Slot;

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
            using full_signature = void( ControlAccess*, XArgs... );

        public:
            /// Unowned signal
            Signal( const std::string& name );
            /// Owned signal
            template< typename XOwner >
            Signal( const std::string& name, XOwner* owner );
            Signal( const Signal& ) = delete;
            Signal( Signal&& ) = delete;
            virtual ~Signal();

            virtual void Connect( SlotBase* slot, int group = -1 );

            // calls all connected functions
            void Emit( XArgs... args );
            void operator()( XArgs... args );

            MEMVAR( bool, DoBreakpoint );
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
            fDoBreakpoint( false )
    {}

    template< typename... XArgs >
    template< typename XOwner >
    inline Signal< XArgs... >::Signal( const std::string& name, XOwner* owner ) :
            SignalBase( name ),
            fDoBreakpoint( false )
    {
        owner->RegisterSignal( name, this );
    }

    template< typename... XArgs >
    Signal< XArgs... >::~Signal()
    {
        DisconnectAll();
    }

    template< typename... XArgs >
    void Signal< XArgs... >::Connect( SlotBase* slot, int group )
    {
        if( fConnections.count( slot ) != 0 )
        {
            LWARN( signallog, "Signal <" << fName << "> is already connected to slot <" << slot->Name() << ">" );
            return;
        }

        // ensure that the slot is of the correct type
        if( ! dynamic_cast< Slot< XArgs... >* >( slot ) )
        {
            THROW_EXCEPT_HERE( ConnectionException() << "Trying to connect signal <" << fName << "> to slot <" << slot->Name() << ">, but cannot make the connection:\n" <<
                    "\tUnable to cast from SlotBase to this signal's derived type.\n" << 
                    "\tArgument types do not match" );

            //BOOST_THROW_EXCEPTION( ConnectionException() << "Trying to connect signal <" << fName << "> to slot <" << slot->Name() << ">, but cannot make the connection:\n" <<
            //        "\tUnable to cast from SlotBase to this signal's derived type.\n" << 
            //        "\tArgument types do not match" << eom );
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
        AddConnection( slot, group );

        return;
    }

    // calls all connected functions
    template< typename... XArgs >
    inline void Signal< XArgs... >::Emit( XArgs... args )
    {
        (*this)( args... );
        return;
    }

    template< typename... XArgs >
    inline void Signal< XArgs... >::operator()( XArgs... args )
    {
        if( fDoBreakpoint )
        {
            fControl->SetReturn< XArgs... >( args... );
            fControl->Break(); // waits for resume or exit
        }

        for( auto connection : fConnections )
        {
            static_cast< Slot< XArgs... >* >(connection)->operator()( fControl, args... );
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
