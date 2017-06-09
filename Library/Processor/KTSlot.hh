/*
 * KTSlot.hh
 *
 *  Created on: Jan 13, 2013
 *      Author: nsoblath
 */

#ifndef KTSLOT_HH_
#define KTSLOT_HH_

#include "KTException.hh"
#include "KTLogger.hh"
#include "KTSignal.hh"

#include <functional>
#include <string>

namespace Nymph
{
    KTLOGGER(slotlog, "KTSlot");

    /*!
     @class KTSlot
     @author N. S. Oblath

     @brief Creates a slot that calls a member function of the func_owner_type object, taking 0 or more arguments.

     @details
     Usage:
     To use this slot type the function to be called by the slot must exist in an object of type FuncOwnerType.
     The function should have the signature ReturnType (Args).

     In your Processor's header add a member variable of type KTSlot< ProcessorType, ReturnType, Args >.
     The variable may be private.

     Initialize the slot with the name of the slot, the address of the owner of the slot function, and the function pointer.
     Optionally, if the Processor is separate from the owner of the slot function, the Processor address is specified as the second argument to the constructor.
    */
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

    // Typedefs for backwards compatibility

    typedef KTSlot<> KTSlotNoArg;

    template < typename Arg1 >
    using KTSlotOneArg = KTSlot< Arg1 >;

    template< typename Arg1, typename Arg2 >
    using KTSlotTwoArg = KTSlot< Arg1, Arg2 >;


    /*!
     @class KTSlotData
     @author N. S. Oblath

     @brief Creates a slot that takes a KTDataHandle object as the argument; the function that gets called should take 0 or more DataType&'s as its argument.

     @details
     Usage:
     This slot type adds the slot function (signature void (KTDataHandle).
     Your processor (or, optionally, a different object) must have a member function with the signature bool (DataType1&, . . .).
     The slot function checks that the provided KTCoreData object contains data of type DataType, and then calls the member function.

     In your Processor's header add a member variable of type KTSlotOneArg< DataType >.
     The variable may be private.

     Initialize the slot with the name of the slot, the address of the owner of the slot function, and the function pointer.
     Optionally, if the Processor is separate from the owner of the slot function, the Processor address is specified as the second argument to the constructor.

     Also optionally, a signal to be emitted after the return of the member function can be specified as the last argument.
    */
    template< class XReturnType, class... XDataArgs >
    class KTSlotData : public KTSlot< KTDataHandle  >
    {
        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& ), KTSignalData* signalPtr );

            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& ) );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& ), KTSignalData* signalPtr );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& ) );

            virtual ~KTSlotData();

            void operator()( KTDataHandle data );

        protected:
            template< typename... DataTypes >
            bool DataPresent( KTDataHandle data );

            //function_signature fFunc;
            std::function< XReturnType (const XDataArgs&..., XReturnType&) > fFunc;

            KTSignalData* fSignalPtr;
    };

    // partial specialization for no new data type
    template< class... XDataArgs >
    class KTSlotData< void, XDataArgs... > : public KTSlot< KTDataHandle  >
    {
        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... ), KTSignalData* signalPtr );

            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... ) );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... ), KTSignalData* signalPtr );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType, class... XFuncDataTypes >
            KTSlotData( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... ) );

            virtual ~KTSlotData();

            void operator()( KTDataHandle data );

        protected:
            template< typename... DataTypes >
            bool DataPresent( KTDataHandle data );

            //function_signature fFunc;
            std::function< void (XDataArgs&...) > fFunc;

            KTSignalData* fSignalPtr;
    };

    // helper structs (have to be defined outside the templated KTSlot class to use specialization
    template< typename... DataTypes >
    struct DataPresentHelper
    {
        static bool DataPresent( KTDataHandle )
        {
            return true;
        }
    };

    template< typename DataType, typename... DataTypes >
    struct DataPresentHelper< DataType, DataTypes... >
    {
        static bool DataPresent( KTDataHandle data )
        {
            if( ! data->Has< DataType >() )
            {
                KTERROR( slotlog, "Data not found with type <" << typeid( DataType ).name() << ">" );
                return false;
            }
            return DataPresentHelper< DataTypes... >::DataPresent( data );
        }
    };


    // Typedefs for backwards compatibility

    //template< typename XDataType1 >
    //using KTSlotDataOneType = KTSlotData< XDataType1 >;

    //template< typename XDataType1, typename XDataType2 >
    //using KTSlotDataTwoTypes = KTSlotData< XDataType1, XDataType2 >;


    /*!
     @class KTSlotDone
     @author N. S. Oblath

     @brief Creates a slot to receive indication that upstream processing is complete and will emit a similar signal.

     @details
     Usage:
     This slot type adds the slot function (signature void ()).
     Your processor (or, optionally, a different object) must have a member function with the signature void ().
     The slot calls the member function.

     In your Processor's header add a member variable of type KTDoneSlot.
     The variable may be private.

     Initialize the slot with the name of the slot, the address of the owner of the slot function, and the function pointer.
     Optionally, if the Processor is separate from the owner of the slot function, the Processor address is specified as the second argument to the constructor.

     Also optionally, a signal to be emitted after the return of the member function can be specified as the last argument.
    */
    class KTSlotDone : public KTSlot<>
    {
        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType >
            KTSlotDone( const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr=nullptr );
            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType >
            KTSlotDone( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr=nullptr );
            virtual ~KTSlotDone();

            void operator()();

        protected:
            boost::function< void () > fFunc;

            KTSignalDone* fSignalPtr;
    };


    //*******************
    // Implementations
    //*******************

    // KTSlot

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

    // KTSlotData

    template< class XReturnType, class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< XReturnType, XDataTypes... >::KTSlotData(const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& ), KTSignalData* signalPtr) :
            KTSlot( name, owner, this, &KTSlotData::operator(), {signalPtr->GetName()} ),
            fFunc( [func, owner]( XDataTypes... args ){ return (owner->*func)(args...);} ),
            fSignalPtr( signalPtr )
    {
    }

    template< class XReturnType, class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< XReturnType, XDataTypes... >::KTSlotData(const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& )) :
            KTSlot( name, owner, this, &KTSlotData::operator() ),
            fFunc( [func, owner]( XDataTypes... args ){ return (owner->*func)(args...);} ),
            fSignalPtr( nullptr )
    {
    }

    template< class XReturnType, class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< XReturnType, XDataTypes... >::KTSlotData(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& ), KTSignalData* signalPtr) :
            KTSlot( name, proc, this, &KTSlotData::operator(), { signalPtr->GetName()} ),
            fFunc( [func, owner]( XDataTypes... args ){return (owner->*func) (args... );} ),
            fSignalPtr( signalPtr )
    {
    }

    template< class XReturnType, class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< XReturnType, XDataTypes... >::KTSlotData(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&..., XReturnType& )) :
            KTSlot( name, proc, this, &KTSlotData::operator() ),
            fFunc( [func, owner]( XDataTypes... args ){return (owner->*func) (args... );} ),
            fSignalPtr( nullptr )
    {
    }

    template< class XReturnType, class... XDataTypes >
    KTSlotData< XReturnType, XDataTypes... >::~KTSlotData()
    {
    }

    template< class XReturnType, class... XDataTypes >
    void KTSlotData< XReturnType, XDataTypes... >::operator()( KTDataHandle dataHandle )
    {
        // Standard data slot pattern:

        std::shared_ptr< KTThreadReference > ref = fSlotWrapper->GetThreadRef();

        // Check to ensure that the required data type is present
        if( ! DataPresent< XDataTypes... >( dataHandle ) )
        {
            KTERROR( slotlog, "Failed to find all of the necessary data types in slot <" << fName << ">. Aborting." );
            THROW_THREADREF_EXCEPTION( ref, KTException() << "Failed to find all of the necessary data types in slot <" << fName << ">. Aborting." );
            return;
        }

        // Call the function
        try
        {
            fFunc( dataHandle->Of< XDataTypes >()... , dataHandle->Of< XReturnType >() );
        }
        catch( boost::exception& e )
        {
            e << KTErrorMsgInfo< struct slotData_RunFunc >( "Something went wrong in slot <" + fName + ">. Aborting." );
            ref->SetReturnException( boost::current_exception() );
        }

        // Perform breakpoint here if necessary (either if initiated here or if stopping here due to a breakpoint elsewhere)
        // Sets the dataHandle into the return
        ref->Break( dataHandle, fSlotWrapper->GetDoBreakpoint() );

        // If there's a signal pointer, emit the signal
        if( fSignalPtr != nullptr )
        {
            (*fSignalPtr)( dataHandle );
        }
        return;
    }

    template< class XReturnType, class... XDataTypes >
    template< typename... DataTypes >
    bool KTSlotData< XReturnType, XDataTypes... >::DataPresent( KTDataHandle data )
    {
        return DataPresentHelper< DataTypes... >::DataPresent( data );
    }


    // KTSlotData with no return type

    template< class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< void, XDataTypes... >::KTSlotData(const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... ), KTSignalData* signalPtr) :
            KTSlot( name, owner, this, &KTSlotData::operator(), {signalPtr->GetName()} ),
            fFunc( [func, owner]( XDataTypes... args ){ return (owner->*func)(args...);} ),
            fSignalPtr( signalPtr )
    {
    }

    template< class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< void, XDataTypes... >::KTSlotData(const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... )) :
            KTSlot( name, owner, this, &KTSlotData::operator() ),
            fFunc( [func, owner]( XDataTypes... args ){ return (owner->*func)(args...);} ),
            fSignalPtr( nullptr )
    {
    }

    template< class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< void, XDataTypes... >::KTSlotData(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... ), KTSignalData* signalPtr) :
            KTSlot( name, proc, this, &KTSlotData::operator(), { signalPtr->GetName()} ),
            fFunc( [func, owner]( XDataTypes... args ){return (owner->*func) (args... );} ),
            fSignalPtr( signalPtr )
    {
    }

    template< class... XDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    KTSlotData< void, XDataTypes... >::KTSlotData(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)( const XFuncDataTypes&... )) :
            KTSlot( name, proc, this, &KTSlotData::operator() ),
            fFunc( [func, owner]( XDataTypes... args ){return (owner->*func) (args... );} ),
            fSignalPtr( nullptr )
    {
    }

    template< class... XDataTypes >
    KTSlotData< void, XDataTypes... >::~KTSlotData()
    {
    }

    template< class... XDataTypes >
    void KTSlotData< void, XDataTypes... >::operator()( KTDataHandle dataHandle )
    {
        // Standard data slot pattern:

        std::shared_ptr< KTThreadReference > ref = fSlotWrapper->GetThreadRef();

        // Check to ensure that the required data type is present
        if( ! DataPresent< XDataTypes... >( dataHandle ) )
        {
            KTERROR( slotlog, "Failed to find all of the necessary data types in slot <" << fName << ">. Aborting." );
            THROW_THREADREF_EXCEPTION( ref, KTException() << "Failed to find all of the necessary data types in slot <" << fName << ">. Aborting." );
            return;
        }

        // Call the function
        try
        {
            fFunc( dataHandle->Of< XDataTypes >()... );
        }
        catch( boost::exception& e )
        {
            e << KTErrorMsgInfo< struct slotData_RunFunc >( "Something went wrong in slot <" + fName + ">. Aborting." );
            ref->SetReturnException( boost::current_exception() );
        }

        // Perform breakpoint here if necessary (either if initiated here or if stopping here due to a breakpoint elsewhere)
        // Sets the dataHandle into the return
        ref->Break( dataHandle, fSlotWrapper->GetDoBreakpoint() );

        // If there's a signal pointer, emit the signal
        if( fSignalPtr != nullptr )
        {
            (*fSignalPtr)( dataHandle );
        }
        return;
    }

    template< class... XDataTypes >
    template< typename... DataTypes >
    bool KTSlotData< void, XDataTypes... >::DataPresent( KTDataHandle data )
    {
        return DataPresentHelper< DataTypes... >::DataPresent( data );
    }


    // KTSlotDone

    template< class XFuncOwnerType >
    KTSlotDone::KTSlotDone(const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr) :
            KTSlot( name, owner, this, &KTSlotDone::operator(), {signalPtr->GetName()} ),
            fFunc( [func, owner](){ return (owner->*func)(); } ),
            fSignalPtr( signalPtr )
    {
    }

    template< class XFuncOwnerType >
    KTSlotDone::KTSlotDone(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr) :
            KTSlot( name, proc, this, &KTSlotDone::operator(), {signalPtr->GetName()} ),
            fFunc( [func, owner](){ return (owner->*func)(); } ),
            fSignalPtr( signalPtr )
    {
    }

    inline KTSlotDone::~KTSlotDone()
    {
    }

    inline void KTSlotDone::operator()()
    {
        // Call the function
        fFunc();

        // Perform breakpoint here if necessary (either if initiated here or if stopping here due to a breakpoint elsewhere)
        // Sets the dataHandle into the return
        fSlotWrapper->GetThreadRef()->Break( KTDataHandle(), fSlotWrapper->GetDoBreakpoint() );

        // If there's a signal pointer, emit the signal
        if( fSignalPtr != nullptr )
        {
            (*fSignalPtr)();
        }
        return;
    }

} /* namespace Nymph */
#endif /* KTSLOT_HH_ */
