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

#include "logger.hh"

#include <boost/function.hpp>

#include <initializer_list>

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
            /// Unowned slot
            Slot( const std::string& name, const boost::function< signature >& func, signal_list signals = {} );
            /// Owned slot, non-const member function
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ), signal_list signals = {} );
            /// Owned slot, const member function
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const, signal_list signals = {} );
            /// Owned slot, generic function
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, const boost::function< signature >& func, signal_list signals = {} );
            /// Owned slot, non-const member fuction of another class
            template< typename XOwner, typename XFuncClass >
            Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ), signal_list signals = {} );
            /// Owned slot, const member fuction of another class
            template< typename XOwner, typename XFuncClass >
            Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const, signal_list signals = {} );
            Slot( const Slot& ) = delete;
            Slot( Slot&& ) = delete;
            virtual ~Slot();

            void ConnectTo( SignalPtr_t signal, int group = -1 );

            // execute fFunction
            void operator()( XArgs... args );

            MEMVAR_REF( boost::function< signature >, Function );

            MEMVAR( bool, DoBreakpoint );
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
    Slot< XArgs... >::Slot( const std::string& name, const boost::function< signature >& func, signal_list signals ) :
            SlotBase( name ),
            fFunction( func ),
            //fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint( false )
    {}

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ), signal_list signals ) :
            SlotBase( name ),
            fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);} ),
            //fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const, signal_list signals ) :
            SlotBase( name ),
            fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);}  ),
            //fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner, const boost::function< signature >& func, signal_list signals ) :
            SlotBase( name ),
            fFunction( func ),
            //fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ), signal_list signals ) :
            SlotBase( name ),
            fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  ),
            //fThreadRef( std::make_shared< ThreadReference >() ),
            fDoBreakpoint(false)
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const, signal_list signals ) :
            SlotBase( name ),
            fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  ),
            //fThreadRef( std::make_shared< ThreadReference >() ),
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
    void Slot< XArgs... >::ConnectTo( SignalPtr_t signal, int group )
    {
        if( fConnections.count( signal ) != 0 )
        {
            LWARN( signallog, "Slot <" << fName << "> is already has connection to signal <" << signal->Name() << ">" );
            return;
        }

        signal->Connect( shared_from_this(), group );

        return;
    }

    template< typename... XArgs >
    inline void Slot< XArgs... >::operator()( XArgs... args )
    {
        fFunction( args... );
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_SLOT_HH_ */
