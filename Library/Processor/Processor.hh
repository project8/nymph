/**
 @file Processor.hh
 @brief Base class for processors
 @author: N. S. Oblath
 @date: Jan 5, 2012
 */

#ifndef NYMPH_PROCESSOR_HH_
#define NYMPH_PROCESSOR_HH_

#include "SignalSlotBase.hh"

#include "factory.hh"
#include "logger.hh"
#include "param.hh"

#include <map>

namespace Nymph
{
    LOGGER(processorlog, "Processor.hh");

    struct ProcessorException : virtual public Exception {};

    class Processor
    {
        protected:
            typedef std::map< std::string, SignalBase* > SignalMap;
            typedef SignalMap::iterator SigMapIt;
            typedef SignalMap::value_type SigMapVal;

            typedef std::map< std::string, SlotBase* > SlotMap;
            typedef SlotMap::iterator SlotMapIt;
            typedef SlotMap::value_type SlotMapVal;

        public:
            Processor( const std::string& name );
            virtual ~Processor();

            template< class XDerivedProc >
            static scarab::registrar< Nymph::Processor, XDerivedProc, const std::string& >* RegisterProcessor( const std::string& name );

        public:
            /// Configure the processor with a param_node
            virtual void Configure( const scarab::param_node& node ) = 0;

            /// Utility function for connecting any signal to any slot
            static void ConnectSignalToSlot( SignalBase* signal, SlotBase* slot, int groupNum=-1 );

            /// Connect a signal in this processor to a slot in a different processor
            void ConnectASlot( const std::string& signalName, Processor& processor, const std::string& slotName, int groupNum=-1 );
            /// Connect a signal in a different processor to a slot in this processor
            void ConnectASignal( Processor& processor, const std::string& signalName, const std::string& slotName, int groupNum=-1 );

            /// Register a signal object with this processor
            void RegisterSignal( std::string name, SignalBase* signal );

            /// Register a slot object with this processor
            void RegisterSlot( std::string name, SlotBase* slot, std::initializer_list< std::string > signals = {} );

            bool GetDoBreakpoint( const std::string& slotName );
            void SetDoBreakpoint( const std::string& slotName, bool flag );

            MEMVAR_REF( std::string, Name );

            MEMVAR_REF_CONST( SignalMap, Signals );
            MEMVAR_REF_CONST( SlotMap, Slots );

            // this is used only to hold pointers to slots waiting for signals
            // the keys are the names of the signals being waited for, and the values are the slot pointers
            typedef std::multimap< std::string, SlotBase* > WaitingSlotMap;
            MEMVAR_REF_CONST( WaitingSlotMap, SlotsWaitingForSignals );

    };


    inline void Processor::ConnectASignal(Processor& processor, const std::string& signalName, const std::string& slotName, int groupNum )
    {
        processor.ConnectASlot( signalName, *this, slotName, groupNum );
        return;
    }

    template< class XDerivedProc >
    scarab::registrar< Processor, XDerivedProc, const std::string& >* Processor::RegisterProcessor( const std::string& name )
    {
        return new scarab::registrar< Processor, XDerivedProc, const std::string& >( name );
    }

#define KT_REGISTER_PROCESSOR(proc_class, proc_name) \
        static ::scarab::registrar< ::Nymph::Processor, proc_class, const std::string& > sProc##proc_class##Registrar( proc_name );

} /* namespace Nymph */

#endif /* NYMPH_PROCESSOR_HH_ */
