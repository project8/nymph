/*
 * SlotData.hh
 *
 *  Created on: Jan 25, 2022
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_SLOTDATA_HH_
#define NYMPH_SLOTDATA_HH_

#include "Slot.hh"

namespace Nymph
{
    template< class... XTypes >
    struct TypeList {};

    template< class... XTypes >
    using In = TypeList< XTypes... >;

    template< class... XTypes >
    using Out = TypeList< XTypes... >;

    template< class XInList, class XOutList >
    class SlotData;

    class Processor;

    /*!
     @class SlotData
     @author N. S. Oblath

     @brief Creates a slot that takes a DataHandle as the argument; the function that gets called should take 0 or more DataType&'s as its argument(s).

     @details
     Usage:
     This slot type includes the slot function with signature `void (DataHandle)`.

     Your processor (or, optionally, a different object) must have a member function with the signature bool ([input and output data-type references]).
     The function that gets called can have both input data types (assumed to be const) and output data types (assumed to be non-const).  The following 
     are all valid examples:
     * void my_func(); // no arguments
     * void my_func(DataType1&); // DataType1 is non-const
     * void my_func(const DataType2&); // DataType2 is const
     * void my_func(const DataType3& const DataType4&, DataType5&); // DataType3 and DataType4 are const; DataType5 is non-const
     Note that const data types (i.e. inputs) should come before non-const types (i.e. outputs) in the function signature,
     and the order of the data types in the `In<>` and `Out<>` type lists must match the order of the types in the function signature.

     The slot function checks that the provided DataFrame object (pointed to by the DataHandle) contains objects of all of the const data types, 
     creates (or retrieves) the objects of the non-const data types, and then calls the member function.

     In your Processor's header add a member variable of type SlotData< In<[input data types]>, Out<[output data types]> >.
     The access specifier for the slot (e.g. protected, private, or public) does not affect the usability of the slot. 

     Initialize the slot with the name of the slot, the address of the owner of the slot function, and the function pointer.
     Optionally, if the Processor is separate from the owner of the slot function, the Processor address is specified as the second argument to the constructor.

     Also optionally, a signal to be emitted after the return of the member function can be specified as the last argument.
    */
    template< template<class...> class XInList, template<class...> class XOutList, class... XInTypes, class... XOutTypes >
    class SlotData< XInList<XInTypes...>, XOutList<XOutTypes...> > : public Slot< DataHandle >
    {
        public:
            template< class XFuncOwnerType >
            using Signature = void (XFuncOwnerType::*)( XInTypes&..., XOutTypes&... );

        public:
            /// Constructor for the case where the processor has the function that will be called by the slot
            template< class XOwnerType >
            SlotData( const std::string& name, XOwnerType* owner, Signature< XOwnerType > func, SignalData* signalPtr = nullptr );

            /// Constructor for the case where the processor and the object with the function that will be called are different
            template< class XSlotOwnerType, class XFuncOwnerType >
            SlotData( const std::string& name, XSlotOwnerType* proc, XFuncOwnerType* inst, Signature< XFuncOwnerType > func, SignalData* signalPtr = nullptr );

            virtual ~SlotData();

            void operator()( DataHandle handle );

        protected:
            //function_signature fFunc;
            std::function< void (XInTypes&..., XOutTypes&... ) > fFunc;

            SignalData* fSignalPtr;
    };


    //*******************
    // Implementations
    //*******************

    template< template<class...> class XInList, template<class...> class XOutList, class... XInTypes, class... XOutTypes >
    template< class XOwnerType >
    SlotData< XInList<XInTypes...>, XOutList<XOutTypes...> >::SlotData(const std::string& name, XOwnerType* owner, SlotData< XInList<XInTypes...>, XOutList<XOutTypes...> >::Signature< XOwnerType > func, SignalData* signalPtr ) :
            Slot( name, owner, this, &SlotData::operator() ),
            fFunc( [func, owner]( XInTypes&... inArgs, XOutTypes&... outArgs ){ (owner->*func)( inArgs..., outArgs... ); } ),
            fSignalPtr( signalPtr )
    {}

    template< template<class...> class XInList, template<class...> class XOutList, class... XInTypes, class... XOutTypes >
    template< class XSlotOwnerType, class XFuncOwnerType >
    SlotData< XInList<XInTypes...>, XOutList<XOutTypes...> >::SlotData(const std::string& name, XSlotOwnerType* owner, XFuncOwnerType* inst, SlotData< XInList<XInTypes...>, XOutList<XOutTypes...> >::Signature< XFuncOwnerType > func, SignalData* signalPtr ) :
            Slot( name, owner, this, &SlotData::operator() ),
            fFunc( [func, inst]( XInTypes&... inArgs, XOutTypes&... outArgs ){ (inst->*func)( inArgs..., outArgs... ); } ),
            fSignalPtr( signalPtr )
    {}

    template< template<class...> class XInList, template<class...> class XOutList, class... XInTypes, class... XOutTypes >
    SlotData< XInList<XInTypes...>, XOutList<XOutTypes...> >::~SlotData()
    {}

    template< template<class...> class XInList, template<class...> class XOutList, class... XInTypes, class... XOutTypes >
    void SlotData< XInList<XInTypes...>, XOutList<XOutTypes...> >::operator()( DataHandle handle )
    {
        // Standard data slot pattern:

        // Check to ensure that the required data type is present
        if( ! handle->Has< XInTypes... >() )
        {
            THROW_EXCEPT_HERE( Exception() << "Failed to find all of the necessary data types in slot <" << fName << ">. Aborting." );
            return;
        }

        // Call the function
        try
        {
            fFunc( handle->Get< XInTypes >()... , handle->Get< XOutTypes >()... );
        }
        catch( scarab::base_exception& )
        {
            THROW_NESTED_EXCEPT_HERE( Exception() << "Something went wrong in slot <" + fName + ">. Aborting." );
            return;
        }

        // If there's a signal pointer, emit the signal
        if( fSignalPtr )
        {
            (*fSignalPtr)( handle );
        }
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_SLOTDATA_HH_ */
