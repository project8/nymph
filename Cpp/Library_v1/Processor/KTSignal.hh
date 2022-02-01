/*
 * KTSignal.hh
 *
 *  Created on: Jan 15, 2013
 *      Author: nsoblath
 */

#ifndef KTSIGNAL_HH_
#define KTSIGNAL_HH_

#include "KTSignalSlotBase.hh"

#include "KTConnection.hh"
#include "KTException.hh"
#include "KTLogger.hh"

#include <map>

namespace Nymph
{
    KTLOGGER( signallog, "KTSignal" );

    struct KTConnectionException : public KTException
    {};

    /// A m_signal object may call multiple slots with the
    /// same signature. You can connect functions to the m_signal
    /// which will be called when the emit() method on the
    /// m_signal object is invoked. Any argument passed to emit()
    /// will be passed to the given functions.
    /// The name "m_signal" was chosen to avoid conflicting with std::signal.
    template< typename... XArgs >
    class KTSignal : public KTSignalBase
    {
        public:
            using signature = void( XArgs... );
            using boost_signal = boost::signals2::signal< signature >;

        public:
            template< typename XOwner >
            KTSignal( const std::string& name, XOwner* owner );
            KTSignal( const KTSignal& ) = delete;
            KTSignal( KTSignal&& ) = delete;
            virtual ~KTSignal();

            virtual void Connect( KTSlotBase* slot, int group );

            // disconnects a previously connected function
            void Disconnect( KTSlotBase* slot ) const;

            // disconnects all previously connected functions
            void DisconnectAll() const;

            // calls all connected functions
            void Emit( XArgs... args );
            void operator()( XArgs... args );

        private:
            typedef std::map< KTSlotBase*, KTConnection > slot_map; // to get around the problem of having a comma inside a macro function argument
            MEMBERVARIABLE_REF_MUTABLE_CONST( slot_map, Slots );

            boost_signal fInternalSignal;
    };


    /*!
     @class KTSignal
     @author N. S. Oblath

     @brief Creates a signal that takes 0 or more arguments.

     @details
     The signal is emitted by calling operator().
     If a KTSlot is being used, and the Slot has been given a pointer to this signal, the Slot will emit the Signal.

     Usage:
     In your Processor's header add a member variable of type KTSignal< ArgumentTypes >.

     Initialize the signal with the processor's 'this' pointer and the name of the signal.

     To use the signal, call it as: fSignalObject(arg);
    *//*
    template< class... XSignalArguments >
    class KTSignal
    {
        public:
            typedef void (signature)( XSignalArguments... );
            typedef boost::signals2::signal< signature > boost_signal;
            typedef typename boost::signals2::signal< signature >::slot_type slot_type;

        public:
            KTSignal();
            KTSignal( const std::string& name, KTProcessor* proc );
            virtual ~KTSignal();

        protected:
            KTSignal( const KTSignal& );

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
    template< typename XOwner >
    KTSignal< XArgs... >::KTSignal< XOwner >( const std::string& name, XOwner* owner ) :
            fSlots(),
            fInternalSignal()
    {
        owner->RegisterSignal( name, this );
    }

    template< typename... XArgs >
    KTSignal< XArgs... >::~KTSignal()
    {
        DisconnectAll();
    }

    template< typename... XArgs >
    void KTSignal< XArgs... >::Connect( KTSlotBase* slot, int group )
    {
        if( fSlots.count( slot ) != 0 )
        {
            KTWARN( signallog, "Signal <" << fName << "> is already connected to slot <" << slot->Name() << ">" );
            return;
        }

        // ensure that the slot is of the correct type
        KTSlot< XArgs... >* derivedSlot = dynamic_cast< KTSlot< XArgs... >* >( slot );
        if( slot == nullptr )
        {
            BOOST_THROW_EXCEPTION( KTConnectionException() << "Trying to connect signal <" << fName << "> to slot <" << slot->Name() << ">, but cannot make the connection:\n" <<
                    "\tUnable to cast from KTSlotBase to this signal's derived type." << eom );
        }

        KTConnection connection;
        if( group >= 0 )
        {
            connection = fInternalSignal.connect( group, derivedSlot->Function() );
        }
        else
        {
            connection = fInternalSignal.connect( derivedSlot->Function() );
        }

        fSlots.insert( std::make_pair< KTSlotBase*, KTConnection >( slot, connection ) );
        slot->AddConnection( this );

        return;
    }

    // disconnects a previously connected function
    template< typename... XArgs >
    void KTSignal< XArgs... >::Disconnect( KTSlotBase* slot ) const
    {
        auto connectionIt = fSlots.find( slot );
        if( connectionIt != fSlots.end() )
        {
            connectionIt->second.disconnect();
            connectionIt->first->RemoveConnection( this );
        }
        return;
    }

    // disconnects all previously connected functions
    template< typename... XArgs >
    void KTSignal< XArgs... >::DisconnectAll() const
    {
        for( auto connection : fSlots )
        {
            connection.second.disconnect();
            connection.first->RemoveConnection( this );
        }
        return;
    }

    // calls all connected functions
    template< typename... XArgs >
    void KTSignal< XArgs... >::Emit( XArgs... args )
    {
        (*this)( args );
        return;
    }

    template< typename... XArgs >
    void KTSignal< XArgs... >::operator()( XArgs... args )
    {
        fSignal( args );
    }

/*
    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::KTSignal( const std::string& name, KTProcessor* proc ) :
            fSignal(),
            fName( name )
    {
        proc->RegisterSignal(name, &fSignal);
    }

    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::KTSignal() :
            fSignal(),
            fName("none")
    {}

    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::KTSignal( const KTSignal& signal ) :
            fSignal(),
            fName( signal.fName )
    {}

    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::~KTSignal()
    {
    }

    template< class... XSignalArguments >
    inline void KTSignal< XSignalArguments... >::operator()( XSignalArguments... args )
    {
        fSignal( args... );
    }

    template< class... XSignalArguments >
    inline typename KTSignal< XSignalArguments... >::boost_signal* KTSignal< XSignalArguments... >::Signal()
    {
        return &fSignal;
    }

    template< class... XSignalArguments >
    inline const std::string& KTSignal< XSignalArguments... >::GetName() const
    {
        return fName;
    }
*/
} /* namespace Nymph */
#endif /* KTSIGNAL_HH_ */
