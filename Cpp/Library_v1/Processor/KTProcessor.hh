/**
 @file KTProcessor.hh
 @brief Contains KTProcessor
 @details KTProcessor is the processor base class
 @author: N. S. Oblath
 @date: Jan 5, 2012
 */

#ifndef KTPROCESSOR_HH_
#define KTPROCESSOR_HH_

#include "KTConfigurable.hh"

#include "KTLogger.hh"
#include "KTNymphSlot.hh"

#include "factory.hh"

#include <exception>
#include <initializer_list>
#include <map>
#include <sstream>
#include <memory>

namespace Nymph
{
    KTLOGGER(processorlog, "KTProcessor.hh");

    struct KTProcessorException : virtual public KTException
    {};

    struct KTSignalException : virtual public KTException {};
    struct KTSlotException : virtual public KTException {};

    class KTThreadReference;

    class KTProcessor : public KTConfigurable
    {
        protected:
            typedef std::map< std::string, KTSignalBase* > SignalMap;
            typedef SignalMap::iterator SigMapIt;
            typedef SignalMap::value_type SigMapVal;

            typedef std::map< std::string, KTSlotBase* > SlotMap;
            typedef SlotMap::iterator SlotMapIt;
            typedef SlotMap::value_type SlotMapVal;

            typedef std::multimap< std::string, std::string > SlotToSigMap;
            typedef SlotToSigMap::iterator SlotToSigMapIt;
            typedef SlotToSigMap::const_iterator SlotToSigMapCIt;
            typedef SlotToSigMap::value_type SlotToSigMapVal;

            typedef std::multimap< std::string, std::pair< KTProcessor*, std::string > > SigConnMap;
            typedef SigConnMap::iterator SigConnMapIt;
            typedef SigConnMap::const_iterator SigConnMapCIt;
            typedef SigConnMap::value_type SigConnMapVal;

        public:
            KTProcessor(const std::string& name="default-proc-name");
            virtual ~KTProcessor();

            template< class XDerivedProc >
            static scarab::registrar< Nymph::KTProcessor, XDerivedProc, const std::string& >* RegisterProcessor( const std::string& name );

        public:
            /// For a slot that is called, update the slot's ThreadRef, and pass the update to any slots that get called by signals emitted by this slot
            void PassThreadRefUpdate(const std::string& slotName, std::shared_ptr< KTThreadReference > threadRef);

            static void ConnectSignalToSlot(KTSignalBase* signal, KTSlotBase* slot, int groupNum=-1);

            void ConnectASlot(const std::string& signalName, KTProcessor* processor, const std::string& slotName, int groupNum=-1);
            void ConnectASignal(KTProcessor* processor, const std::string& signalName, const std::string& slotName, int groupNum=-1);

            void RegisterSignal(std::string name, KTSignalBase* signal);

            void RegisterSlot(std::string name, KTSlotBase* slot, std::initializer_list< std::string > signals = {});

            KTSignalBase* GetSignal(const std::string& name);

            KTSlotBase* GetSlot(const std::string& name);

            bool GetDoBreakpoint(const std::string& slotName);
            void SetDoBreakpoint(const std::string& slotName, bool flag);

        protected:
            template< typename XReturn, typename... XArgs >
            void PassToConnProcs(const std::string& slotName, std::function< XReturn(XArgs...) > function, XArgs... args);

            SignalMap fSignalMap;

            SlotMap fSlotMap;

            // maps which slots call which signals in this processor
            SlotToSigMap fSlotToSigMap;

            // maps which signals get connected to which slots in other processors
            SigConnMap fSigConnMap;
    };


    void KTProcessor::RegisterSignal(std::string name, KTSignalBase* signal)
    {
        KTDEBUG(processorlog, "Registering signal <" << name << "> in processor <" << fConfigName << ">");
        fSignalMap.insert(SigMapVal(name, signal));
        return;
    }

    void KTProcessor::RegisterSlot(std::string name, KTSlotBase* slot, std::initializer_list< std::string > signals)
    {
        KTDEBUG(processorlog, "Registering slot <" << name << "> in processor <" << fConfigName << ">");

        fSlotMap.insert(SlotMapVal(name, slot));

        for (auto sigIt = signals.begin(); sigIt != signals.end(); ++sigIt)
        {
            fSlotToSigMap.insert(SlotToSigMapVal(name, *sigIt));
            KTDEBUG(processorlog, "Slot-to-signal connection <" << name << "> --> <" << *sigIt << ">");
        }
        return;
    }

    inline void KTProcessor::ConnectASignal(KTProcessor* processor, const std::string& signalName, const std::string& slotName, int groupNum)
    {
        processor->ConnectASlot(signalName, this, slotName, groupNum);
        return;
    }

    template< typename XReturn, typename... XArgs >
    void KTProcessor::PassToConnProcs(const std::string& slotName, std::function< XReturn(XArgs...) > function, XArgs... args)
    {
        // update this slot
        function(args...);

        // get the list of slot-to-signal connections for this slot
        auto stsRange = fSlotToSigMap.equal_range(slotName);

        // loop over signals called in the performance of slot slotName
        for (SlotToSigMapCIt stsIt = stsRange.first; stsIt != stsRange.second; ++stsIt)
        {
            // loop over all processor:slots called by this signal
            auto sigConnRange = fSigConnMap.equal_range(stsIt->second);
            for (SigConnMapCIt sigConnIt = sigConnRange.first; sigConnIt != sigConnRange.second; ++sigConnIt)
            {
                // pass the update on to the connected-to processor
                sigConnIt->second.first->PassToConnProcs(sigConnIt->second.second, function, args...);
            }
        }
        return;
    }

    template< class XDerivedProc >
    scarab::registrar< KTProcessor, XDerivedProc, const std::string& >* KTProcessor::RegisterProcessor( const std::string& name )
    {
        return new scarab::registrar< KTProcessor, XDerivedProc, const std::string& >( name );
    }

#define KT_REGISTER_PROCESSOR(proc_class, proc_name) \
        static ::scarab::registrar< ::Nymph::KTProcessor, proc_class, const std::string& > sProc##proc_class##Registrar( proc_name );

} /* namespace Nymph */
#endif /* KTPROCESSOR_HH_ */
