/*
 * Slot.hh
 *
 *  Created on: Jan 13, 2013
 *      Author: N.S. Oblath
 * 
 *  Based on this implementation: https://schneegans.github.io/tutorials/2015/09/20/signal-slot
 */

#ifndef NYMPH_SLOT_HH_
#define NYMPH_SLOT_HH_

#include "SignalSlotBase.hh"

//#include "Exception.hh"
//#include "ThreadReference.hh"

#include "Logger.hh"

#include <initializer_list>
#include <set>

namespace Nymph
{
    LOGGER(slotlog, "Slot");


    // Type XOwner is the class that owns the Slot object
    template< typename... XArgs >
    class Slot : public SlotBase
    {
        public:
            using signature = void( XArgs... );
            using signal_list = std::initializer_list< std::string >;

        public:
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ), signal_list signals = {} );
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const, signal_list signals = {} );
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, const boost::function< signature >& func, signal_list signals = {} );
            template< typename XOwner, typename XFuncClass >
            Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ), signal_list signals = {} );
            template< typename XOwner, typename XFuncClass >
            Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const, signal_list signals = {} );
            Slot( const Slot& ) = delete;
            Slot( Slot&& ) = delete;
            virtual ~Slot();

            void DisconnectAll() const;

            void AddConnection( SignalPtr_t ) const;

            void RemoveConnection( SignalPtr_t ) const;

            MEMVAR_REF( boost::function< signature >, Function );

            typedef std::set< SignalBase* > signal_connections; // to get around the problem of having a comma inside a macro function argument
            MEMVAR_REF_MUTABLE( signal_connections, Connections );

            MEMVAR( bool, DoBreakpoint );

        protected:
            friend class Signal< XArgs... >;
            void _AddConnection( Signal_ptr_t ) const;

    };


    /*!
     @class Slot
     @author N. S. Oblath

     @brief Creates a slot that calls a member function of the func_owner_type object, taking 0 or more arguments.

     @details
     Usage:
     To use this slot type the function to be called by the slot must exist in an object of type FuncOwnerType.
     The function should have the signature void (Args).

     In your Processor's header add a member variable of type Slot< ProcessorType, ReturnType, Args >.
     The variable may be private.

     Initialize the slot with the name of the slot, the address of the owner of the slot function, and the function pointer.
     Optionally, if the Processor is separate from the owner of the slot function, the Processor address is specified as the second argument to the constructor.
    *//*
    template< typename... Args >
    class Slot
    {
        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType >
            Slot( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( Args... ), std::initializer_list< std::string > signals = {} );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType >
            Slot( const std::string& name, Processor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( Args... ), std::initializer_list< std::string > signals = {} );

            virtual ~Slot();

            const std::string& GetName() const;

            SlotWrapper* GetSlotWrapper();

        protected:
            std::string fName;
            SlotWrapper* fSlotWrapper;
    };
*/


    //*******************
    // Implementations
    //*******************

    // Slot

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot< XOwner >( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ), signal_list signals ) :
            fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);} ),
            fConnections(),
            fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot< XOwner >( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const, signal_list signals ) :
            fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);}  ),
            fConnections(),
            fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot< XOwner >( const std::string& name, XOwner* owner, const boost::function< signature >& func, signal_list signals ) :
            fFunction( func ),
            fConnections(),
            fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    Slot< XArgs... >::Slot< XOwner, XFuncClass >( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ), signal_list signals ) :
            fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  ),
            fConnections(),
            fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    Slot< XArgs... >::Slot< XOwner, XFuncClass >( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const, signal_list signals ) :
            fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  ),
            fConnections(),
            fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    Slot< XArgs... >::~Slot()
    {
        DisconnectAll();
    }

    template< typename... XArgs >
    void Slot< XArgs... >::DisconnectAll() const
    {
        for( auto connection : fConnections )
        {
            connection->Disconnect( this );
        }
        return;
    }

    template< typename... XArgs >
    void Slot< XArgs... >::AddConnection( SignalPtr_t signal ) const
    {
        if( fConnections.count( signal ) != 0 )
        {
            LWARN( signallog, "Slot <" << fName << "> is already has connection to signal <" << signal->Name() << ">" );
            return;
        }

        // ensure that the slot is of the correct type
        if( ! std::dynamic_pointer_cast< Signal< XArgs... > >( signal ) )
        {
            BOOST_THROW_EXCEPTION( ConnectionException() << "Trying to connect slot <" << fName << "> to signal <" << signal->Name() << ">, but cannot make the connection:\n" <<
                    "\tUnable to cast from SignalBase to this slot's derived type.\n" << 
                    "\tArgument types do not match" << eom );
        }

        _AddConnection( signal, group );
        signal->_Connect( this, group );

        return;
    }

    template< typename... XArgs >
    void Slot< XArgs... >::_AddConnection( SignalPtr_t signal ) const
    {
        fConnections.insert( signal );
        return;
    }

    template< typename... XArgs >
    void Slot< XArgs... >::RemoveConnection( SignalPtr_t signal ) const
    {
        fConnections.erase( signal );
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_SLOT_HH_ */
