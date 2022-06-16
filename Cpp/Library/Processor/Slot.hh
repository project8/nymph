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

#include "logger.hh"

//#include <boost/function.hpp>
#include <functional>

namespace Nymph
{
    LOGGER(slotlog, "Slot");

    // Type XOwner is the class that owns the Slot object
    /*!
     @class Slot
     @author N. S. Oblath

     @brief A Slot is connected to and called by a Signal with the same signature.  Slots call a particular function when they're triggered (usually by a signal).

     @details
     Usage:
     The different constructors allow different relationships between the owner of the slot (if there is one) and the owner of the function to be called (if there is one)
     The function should have the signature void (Args).
    */
    template< typename... XArgs >
    class Slot : public SlotBase
    {
        public:
            using signature = void( XArgs... );

        public:
            /// Unowned slot
            Slot( const std::string& name, const std::function< signature >& func );
            /// Owned slot, non-const member function
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) );
            /// Owned slot, const member function
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const );
            /// Owned slot, generic function
            template< typename XOwner >
            Slot( const std::string& name, XOwner* owner, const std::function< signature >& func );
            /// Owned slot, non-const member fuction of another class
            template< typename XOwner, typename XFuncClass >
            Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) );
            /// Owned slot, const member fuction of another class
            template< typename XOwner, typename XFuncClass >
            Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const );
            Slot( const Slot& ) = delete;
            Slot( Slot&& ) = delete;
            virtual ~Slot();

            void ConnectTo( SignalBase* signal, int group = -1 );

            /// execute fFunction
            void operator()( XArgs... args );

            MEMVAR_REF( std::function< signature >, Function );

    };

    //*******************
    // Implementations
    //*******************

    // Slot

    template< typename... XArgs >
    Slot< XArgs... >::Slot( const std::string& name, const std::function< signature >& func ) :
            SlotBase( name ),
            fFunction( func )
    {}

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) ) :
            SlotBase( name, owner ),
            fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);} )
    {}

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const ) :
            SlotBase( name, owner ),
            fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);}  )
    {}

    template< typename... XArgs >
    template< typename XOwner >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner, const std::function< signature >& func ) :
            SlotBase( name, owner ),
            fFunction( func )
    {}

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) ) :
            SlotBase( name, owner ),
            fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  )
    {}

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    Slot< XArgs... >::Slot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const ) :
            SlotBase( name, owner ),
            fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  )
    {}

    template< typename... XArgs >
    Slot< XArgs... >::~Slot()
    {}

    template< typename... XArgs >
    void Slot< XArgs... >::ConnectTo( SignalBase* signal, int group )
    {
        if( fConnections.count( signal ) != 0 )
        {
            LWARN( signallog, "Slot <" << fName << "> is already has connection to signal <" << signal->Name() << ">" );
            return;
        }

        signal->Connect( this, group );

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
