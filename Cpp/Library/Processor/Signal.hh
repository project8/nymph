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

#include "ControlAccess.hh"
#include "QuitChain.hh"

#include "logger.hh"


namespace Nymph
{
    LOGGER( signallog, "Signal" );

    template< typename... XArgs >
    class Slot;

    /*!
     @class Signal
     @author N. S. Oblath

     @brief A signal object may call multiple slots with the same signature.  You can connect functions to the Signal 
     that will be called when the `emit()` method on the Signal object is invoked.  Any argument passed to `emit()`
     will be passed to the given functions.

     @details
     An owned slot will register itself with the owner.  The owner must have a `RegisterSignal()` function that can be called.
     
    */
    template< typename... XArgs >
    class Signal : public SignalBase
    {
        public:
            using signature = void( XArgs... );

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
    };


    //*******************
    // Implementations
    //*******************

    template< typename... XArgs >
    Signal< XArgs... >::Signal( const std::string& name ) :
            SignalBase( name )
    {}

    template< typename... XArgs >
    template< typename XOwner >
    inline Signal< XArgs... >::Signal( const std::string& name, XOwner* owner ) :
            SignalBase( name, owner )
    {}

    template< typename... XArgs >
    Signal< XArgs... >::~Signal()
    {}

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
        }

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
        ControlAccess* control = ControlAccess::get_instance();

        // Check for whether we need to quit from external input:
        // - if we're canceled, then quit the thread
        // - if we're at a break, then wait to continue;
        //     once we continue, if we need to quit, then do so
        if( control->IsCanceled() || (control->IsAtBreak() && ! control->WaitToContinue()) )
        {
            QUIT_CHAIN; // throws QuitThread; should be caught by PrimaryProcessor::operator()
        }

        // Check for whether this signal emission has a breakpoint
        if( fDoBreakpoint )
        {
            // do the break
            LDEBUG( signallog, "Doing breakpoint; will now break and return" );
            control->BreakAndReturn( args... );
            //control->Break();
            // wait to continue; once we continue, if we need to quit, then do so
            LDEBUG( signallog, "Will now wait to continue" );
            if( ! control->WaitToContinue() )
            {
                QUIT_CHAIN; // throws QuitThread; should be caught by PrimaryProcessor::operator()
            }
        }

        // Emit signal by calling all connected slots
        for( auto connection : fConnections )
        {
            static_cast< Slot< XArgs... >* >(connection)->operator()( args... );
        }
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_SIGNAL_HH_ */
