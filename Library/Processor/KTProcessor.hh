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

    class KTProcessor : public KTConfigurable
    {
        protected:
            typedef std::map< std::string, KTSignalWrapper* > SignalMap;
            typedef SignalMap::iterator SigMapIt;
            typedef SignalMap::value_type SigMapVal;

            typedef std::map< std::string, KTSlotWrapper* > SlotMap;
            typedef SlotMap::iterator SlotMapIt;
            typedef SlotMap::value_type SlotMapVal;

        public:
            KTProcessor(const std::string& name="default-proc-name");
            virtual ~KTProcessor();

        public:

            void ConnectASlot(const std::string& signalName, KTProcessor* processor, const std::string& slotName, int groupNum=-1);
            void ConnectASignal(KTProcessor* processor, const std::string& signalName, const std::string& slotName, int groupNum=-1);
            void ConnectSignalToSlot(KTSignalWrapper* signal, KTSlotWrapper* slot, int groupNum=-1);

            template< class XProcessor >
            void RegisterSignal(std::string name, XProcessor* signalPtr);

            template< class XTarget, typename XReturn, typename... XArgs >
            void RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArgs...));

            KTSignalWrapper* GetSignal(const std::string& name);

            KTSlotWrapper* GetSlot(const std::string& name);

        protected:

            SignalMap fSignalMap;

            SlotMap fSlotMap;

    };


    template< typename XSignalSig >
    void KTProcessor::RegisterSignal(std::string name, XSignalSig* signalPtr)
    {
        KTDEBUG(processorlog, "Registering signal <" << name << "> in processor <" << fConfigName << ">");
        KTSignalWrapper* sig = new KTSignalWrapper(signalPtr);
        fSignalMap.insert(SigMapVal(name, sig));
        return;
    }

    template< class XTarget, typename XReturn, typename... XArgs >
    void KTProcessor::RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArgs...))
    {
        KTDEBUG(processorlog, "Registering slot <" << name << "> in processor <" << fConfigName << ">");

        KTSignalConcept< XReturn (XArgs...) > signalConcept;

        KTSlotWrapper* slot = new KTSlotWrapper([funcPtr, target](XArgs... args){return (target->*funcPtr)(args...);}, &signalConcept);
        fSlotMap.insert(SlotMapVal(name, slot));
        return;
    }

#define KT_REGISTER_PROCESSOR(proc_class, proc_name) \
        static ::scarab::registrar< ::Nymph::KTProcessor, proc_class, const std::string& > sProc##proc_class##Registrar( proc_name );

} /* namespace Nymph */
#endif /* KTPROCESSOR_HH_ */
