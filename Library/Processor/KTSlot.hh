/*
 * KTSlot.hh
 *
 *  Created on: Jan 13, 2013
 *      Author: nsoblath
 */

#ifndef KTSLOT_HH_
#define KTSLOT_HH_

#include "KTSignalSlotBase.hh"

//#include "KTException.hh"
#include "KTLogger.hh"

#include <initializer_list>
#include <set>

namespace Nymph
{
    KTLOGGER(slotlog, "KTSlot");


    // Type XOwner is the class that owns the KTSlot object
    template< typename... XArgs >
    class KTSlot : public KTSlotBase
    {
        public:
            using signature = void( XArgs... );
            using signal_list = std::initializer_list< std::string >;

        public:
            template< typename XOwner >
            KTSlot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ), signal_list signals = {} );
            template< typename XOwner >
            KTSlot( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const, signal_list signals = {} );
            template< typename XOwner >
            KTSlot( const std::string& name, XOwner* owner, const boost::function< signature >& func, signal_list signals = {} );
            template< typename XOwner, typename XFuncClass >
            KTSlot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ), signal_list signals = {} );
            template< typename XOwner, typename XFuncClass >
            KTSlot( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const, signal_list signals = {} );
            KTSlot( const KTSlot& ) = delete;
            KTSlot( KTSlot&& ) = delete;
            virtual ~KTSlot();

            void DisconnectAll() const;

            void AddConnection( KTSignalBase* ) const;

            void RemoveConnection( KTSignalBase* ) const;

            MEMBERVARIABLE_REF( boost::function< signature >, Function );

            typedef std::set< KTSignalBase* > signal_connections; // to get around the problem of having a comma inside a macro function argument
            MEMBERVARIABLE_REF_MUTABLE( signal_connections, Connections );

    };


    /*!
     @class KTSlot
     @author N. S. Oblath

     @brief Creates a slot that calls a member function of the func_owner_type object, taking 0 or more arguments.

     @details
     Usage:
     To use this slot type the function to be called by the slot must exist in an object of type FuncOwnerType.
     The function should have the signature void (Args).

     In your Processor's header add a member variable of type KTSlot< ProcessorType, ReturnType, Args >.
     The variable may be private.

     Initialize the slot with the name of the slot, the address of the owner of the slot function, and the function pointer.
     Optionally, if the Processor is separate from the owner of the slot function, the Processor address is specified as the second argument to the constructor.
    *//*
    template< typename... Args >
    class KTSlot
    {
        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType >
            KTSlot( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( Args... ), std::initializer_list< std::string > signals = {} );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType >
            KTSlot( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( Args... ), std::initializer_list< std::string > signals = {} );

            virtual ~KTSlot();

            const std::string& GetName() const;

            KTSlotWrapper* GetSlotWrapper();

        protected:
            std::string fName;
            KTSlotWrapper* fSlotWrapper;
    };
*/


    //*******************
    // Implementations
    //*******************

    // KTSlot

    template< typename... XArgs >
    template< typename XOwner >
    KTSlot< XArgs... >::KTSlot< XOwner >( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ), signal_list signals ) :
            fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);} ),
            fConnections()
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner >
    KTSlot< XArgs... >::KTSlot< XOwner >( const std::string& name, XOwner* owner, void (XOwner::*func)( XArgs... ) const, signal_list signals ) :
    fFunction( [func, owner]( XArgs... args ){ return (owner->*func)(args...);}  ),
    fConnections()
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner >
    KTSlot< XArgs... >::KTSlot< XOwner >( const std::string& name, XOwner* owner, const boost::function< signature >& func, signal_list signals ) :
    fFunction( func ),
    fConnections()
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    KTSlot< XArgs... >::KTSlot< XOwner, XFuncClass >( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ), signal_list signals ) :
    fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  ),
    fConnections()
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    template< typename XOwner, typename XFuncClass >
    KTSlot< XArgs... >::KTSlot< XOwner, XFuncClass >( const std::string& name, XOwner* owner,  XFuncClass *inst, void (XFuncClass::*func)( XArgs... ) const, signal_list signals ) :
    fFunction( [func, inst]( XArgs... args ){ return (inst->*func)(args...);}  ),
    fConnections()
    {
        owner->RegisterSlot( name, this, signals );
    }

    template< typename... XArgs >
    KTSlot< XArgs... >::~KTSlot()
    {
        DisconnectAll();
    }

    template< typename... XArgs >
    void KTSlot< XArgs... >::DisconnectAll() const
    {
        for( auto connection : fConnections )
        {
            connection->Disconnect( this );
        }
        return;
    }

    template< typename... XArgs >
    void KTSlot< XArgs... >::AddConnection( KTSignalBase* signal ) const
    {
        fConnections.insert( signal );
        return;
    }

    template< typename... XArgs >
    void KTSlot< XArgs... >::RemoveConnection( KTSignalBase* signal ) const
    {
        fConnections.erase( signal );
        return;
    }

/*
    template< typename... Args >
    template< class XFuncOwnerType >
    KTSlot< Args... >::KTSlot( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( Args... ), std::initializer_list< std::string > signals ):
            fName( name )
    {
        fSlotWrapper = owner->RegisterSlot( name, owner, func, signals );
    }

    template< typename... Args >
    template< class XFuncOwnerType >
    KTSlot< Args... >::KTSlot( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( Args... ), std::initializer_list< std::string > signals ) :
            fName( name )
    {
        fSlotWrapper = proc->RegisterSlot( name, owner, func, signals );
    }

    template< typename... Args >
    KTSlot< Args... >::~KTSlot()
    {
    }

    template< typename... Args >
    inline const std::string& KTSlot< Args... >::GetName() const
    {
        return fName;
    }

    template< typename... Args >
    inline KTSlotWrapper* KTSlot< Args... >::GetSlotWrapper()
    {
        return fSlotWrapper;
    }
*/
} /* namespace Nymph */
#endif /* KTSLOT_HH_ */
