/*
 * KTSlot.hh
 *
 *  Created on: Jan 13, 2013
 *      Author: nsoblath
 */

#ifndef KTSLOT_HH_
#define KTSLOT_HH_

#include "KTData.hh"
#include "KTLogger.hh"
#include "KTSignal.hh"

#include <functional>
#include <signal.h>
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
    template< typename ReturnType, typename... Args >
    class KTSlot
    {
        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType >
            KTSlot( const std::string& name, XFuncOwnerType* owner, ReturnType (XFuncOwnerType::*func)( Args... ) );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType >
            KTSlot( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, ReturnType (XFuncOwnerType::*func)( Args... ) );

            virtual ~KTSlot();

    };

    // Typedefs for backwards compatibility

    template < typename ReturnType >
    using KTSlotNoArg = KTSlot< ReturnType >;

    template < typename ReturnType, typename Arg1 >
    using KTSlotOneArg = KTSlot< ReturnType, Arg1 >;

    template< typename ReturnType, typename Arg1, typename Arg2 >
    using KTSlotTwoArg = KTSlot< ReturnType, Arg1, Arg2 >;


    /*!
     @class KTDataSlot
     @author N. S. Oblath

     @brief Creates a slot that takes a KTDataPtr object as the argument; the function that gets called should take 0 or more DataType&'s as its argument.

     @details
     Usage:
     This slot type adds the slot function (signature void (KTDataPtr).
     Your processor (or, optionally, a different object) must have a member function with the signature bool (DataType1&, . . .).
     The slot function checks that the provided KTData object contains data of type DataType, and then calls the member function.

     In your Processor's header add a member variable of type KTSlotOneArg< DataType >.
     The variable may be private.

     Initialize the slot with the name of the slot, the address of the owner of the slot function, and the function pointer.
     Optionally, if the Processor is separate from the owner of the slot function, the Processor address is specified as the second argument to the constructor.

     Also optionally, a signal to be emitted after the return of the member function can be specified as the last argument.
    */
    template< class... XDataTypes >
    class KTSlotData
    {
        //public:
            //typedef XDataType data_type;
            //typedef std::function< bool( KTDataPtr ) > function_signature;
            //typedef typename function_signature::result_type return_type;

        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType >
            KTSlotData( const std::string& name, XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( XDataTypes&... ), KTSignalData* signalPtr=nullptr );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType >
            KTSlotData( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( XDataTypes&... ), KTSignalData* signalPtr=nullptr );

            virtual ~KTSlotData();

            void operator()( KTDataPtr data );

        protected:
            template< typename... DataTypes >
            bool DataPresent( KTDataPtr data );

            //template<  >
            //bool DataPresent( KTDataPtr data );
            //template< typename DataType, typename... DataTypes >
            //bool DataPresent( KTDataPtr data );

            //function_signature fFunc;
            std::function< bool (XDataTypes&...) > fFunc;

            KTSignalData* fSignalPtr;
    };

    // helper structs (have to be defined outside the templated KTSlot class to use specialization
    template< typename... DataTypes >
    struct DataPresentHelper
    {
        static bool DataPresent( KTDataPtr )
        {
            return true;
        }
    };

    template< typename DataType, typename... DataTypes >
    struct DataPresentHelper< DataType, DataTypes... >
    {
        static bool DataPresent( KTDataPtr data )
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

    template< typename XDataType1 >
    using KTSlotDataOneType = KTSlotData< XDataType1 >;

    template< typename XDataType1, typename XDataType2 >
    using KTSlotDataTwoTypes = KTSlotData< XDataType1, XDataType2 >;


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
    class KTSlotDone
    {
        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XFuncOwnerType >
            KTSlotDone(const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr=NULL);
            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XFuncOwnerType >
            KTSlotDone(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr=NULL);
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

    template< typename ReturnType, typename... Args >
    template< class XFuncOwnerType >
    KTSlot< ReturnType, Args... >::KTSlot( const std::string& name, XFuncOwnerType* owner, ReturnType (XFuncOwnerType::*func)( Args... ) )
    {
        owner->RegisterSlot( name, owner, func );
    }

    template< typename ReturnType, typename... Args >
    template< class XFuncOwnerType >
    KTSlot< ReturnType, Args... >::KTSlot( const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, ReturnType (XFuncOwnerType::*func)( Args... ) )
    {
        proc->RegisterSlot( name, owner, func );
    }

    template< typename ReturnType, typename... Args >
    KTSlot< ReturnType, Args... >::~KTSlot()
    {
    }


    // KTSlotData

    template< class... XDataTypes >
    template< class XFuncOwnerType >
    KTSlotData< XDataTypes... >::KTSlotData(const std::string& name, XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( XDataTypes&... ), KTSignalData* signalPtr) :
            fFunc([func, owner](XDataTypes... args){return (owner->*func)(args...);}),
            fSignalPtr(signalPtr)
    {
        owner->RegisterSlot(name, this, &KTSlotData::operator());
    }

    template< class... XDataTypes >
    template< class XFuncOwnerType >
    KTSlotData< XDataTypes... >::KTSlotData(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( XDataTypes&... ), KTSignalData* signalPtr) :
            fFunc([func, owner](XDataTypes... args){return (owner->*func)(args...);}),
            fSignalPtr(signalPtr)
    {
        proc->RegisterSlot(name, this, &KTSlotData::operator());
    }

    template< class... XDataTypes >
    KTSlotData< XDataTypes... >::~KTSlotData()
    {
    }

    template< class... XDataTypes >
    void KTSlotData< XDataTypes... >::operator()( KTDataPtr data )
    {
        // Standard data slot pattern:
        // Check to ensure that the required data type is present

        if (! DataPresent< XDataTypes... >( data ) )
        {
            KTFATAL( slotlog, "Failed to find all of the necessary data types in the standard slot function. Aborting." );
            raise(SIGINT);
            return;
        }
        // Call the function
        if ( ! fFunc(data->Of< XDataTypes >()...) )
        {
            KTFATAL( slotlog, "Something went wrong in the standard slot function. Aborting." );
            raise(SIGINT);
            return;
        }
        // If there's a signal pointer, emit the signal
        if (fSignalPtr != NULL)
        {
            (*fSignalPtr)(data);
        }
        return;
    }

    template< class... XDataTypes >
    template< typename... DataTypes >
    bool KTSlotData< XDataTypes... >::DataPresent( KTDataPtr data )
    {
        return DataPresentHelper< DataTypes... >::DataPresent( data );
    }


    // KTSlotDone

    template< class XFuncOwnerType >
    KTSlotDone::KTSlotDone(const std::string& name, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr) :
            fFunc([func, owner](){return (owner->*func)();}),
            fSignalPtr(signalPtr)
    {
        owner->RegisterSlot(name, this, &KTSlotDone::operator());
    }

    template< class XFuncOwnerType >
    KTSlotDone::KTSlotDone(const std::string& name, KTProcessor* proc, XFuncOwnerType* owner, void (XFuncOwnerType::*func)(), KTSignalDone* signalPtr) :
            fFunc([func, owner](){return (owner->*func)();}),
            fSignalPtr(signalPtr)
    {
        proc->RegisterSlot(name, this, &KTSlotDone::operator());
    }

    inline KTSlotDone::~KTSlotDone()
    {
    }

    inline void KTSlotDone::operator()()
    {
        // Call the function
        fFunc();

        // If there's a signal pointer, emit the signal
        if (fSignalPtr != NULL)
        {
            (*fSignalPtr)();
        }
        return;
    }

} /* namespace Nymph */
#endif /* KTSLOT_HH_ */
