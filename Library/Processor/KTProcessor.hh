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

#include "KTConnection.hh"
#include "KTLogger.hh"
#include "KTSignalWrapper.hh"
#include "KTSlotWrapper.hh"

#include "factory.hh"

#include <boost/signals2.hpp>

#include <exception>
#include <initializer_list>
#include <map>
#include <sstream>
#include <string>

namespace Nymph
{
    KTLOGGER(processorlog, "KTProcessor.hh");

    class ProcessorException : public std::logic_error
    {
        public:
            ProcessorException(std::string const& why);
    };

    class KTThreadReference;

    class KTProcessor : public KTConfigurable
    {
        protected:
            typedef std::map< std::string, KTSignalWrapper* > SignalMap;
            typedef SignalMap::iterator SigMapIt;
            typedef SignalMap::value_type SigMapVal;

            typedef std::map< std::string, KTSlotWrapper* > SlotMap;
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
            void PassThreadRefUpdate(const std::string& slotName, KTThreadReference* threadRef);

            void ConnectASlot(const std::string& signalName, KTProcessor* processor, const std::string& slotName, int groupNum=-1);
            void ConnectASignal(KTProcessor* processor, const std::string& signalName, const std::string& slotName, int groupNum=-1);

            template< class XProcessor >
            KTSignalWrapper* RegisterSignal(std::string name, XProcessor* signalPtr);

            template< class XTarget, typename XReturn, typename... XArgs >
            KTSlotWrapper* RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArgs...), std::initializer_list< std::string > signals);

            KTSignalWrapper* GetSignal(const std::string& name);

            KTSlotWrapper* GetSlot(const std::string& name);

            bool GetDoBreakpoint(const std::string& slotName);
            void SetDoBreakpoint(const std::string& slotName, bool flag);

        protected:
            void ConnectSignalToSlot(KTSignalWrapper* signal, KTSlotWrapper* slot, int groupNum=-1);

            SignalMap fSignalMap;

            SlotMap fSlotMap;

            // maps which slots call which signals in this processor
            SlotToSigMap fSlotToSigMap;

            // maps which signals get connected to which slots in other processors
            SigConnMap fSigConnMap;
    };


    template< typename XSignalSig >
    KTSignalWrapper* KTProcessor::RegisterSignal(std::string name, XSignalSig* signalPtr)
    {
        KTDEBUG(processorlog, "Registering signal <" << name << "> in processor <" << fConfigName << ">");
        KTSignalWrapper* sig = new KTSignalWrapper(signalPtr);
        fSignalMap.insert(SigMapVal(name, sig));
        return sig;
    }

    template< class XTarget, typename XReturn, typename... XArgs >
    KTSlotWrapper* KTProcessor::RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArgs...), std::initializer_list< std::string > signals)
    {
        KTDEBUG(processorlog, "Registering slot <" << name << "> in processor <" << fConfigName << ">");

        KTSignalConcept< XReturn (XArgs...) > signalConcept;

        KTSlotWrapper* slot = new KTSlotWrapper([funcPtr, target](XArgs... args){return (target->*funcPtr)(args...);}, &signalConcept);
        fSlotMap.insert(SlotMapVal(name, slot));

        for (std::initializer_list< std::string >::const_iterator sigIt = signals.begin(); sigIt != signals.end(); ++sigIt)
        {
            fSlotToSigMap.insert(SlotToSigMapVal(name, *sigIt));
            KTDEBUG(processorlog, "Slot-to-signal connection <" << name << "> --> <" << *sigIt << ">");
        }
        return slot;
    }

    inline void KTProcessor::ConnectASignal(KTProcessor* processor, const std::string& signalName, const std::string& slotName, int groupNum)
    {
        processor->ConnectASlot(signalName, this, slotName, groupNum);
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
