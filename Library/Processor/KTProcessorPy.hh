/**
 @file KTProcessorPy.hh
 @brief Contains KTProcessor python wrapping
 @details KTProcessor is the processor base class
 @author: B. H. LaRoque
 @date: April 20, 2017
 */

#ifndef KTPROCESSORPY_HH_
#define KTPROCESSORPY_HH_

#include "KTProcessor.hh"

void export_Processor()
{
    using namespace Nymph;
    using namespace boost::python;
    class_<KTProcessor, boost::noncopyable>("KTProcessor", no_init)
        ;
}

/*{
    class KTProcessor : public KTConfigurable
    {
        public:
            KTProcessor(const std::string& name="default-proc-name");
            virtual ~KTProcessor();

        public:

            void ConnectASlot(const std::string& signalName, KTProcessor* processor, const std::string& slotName, int groupNum=-1);
            void ConnectASignal(KTProcessor* processor, const std::string& signalName, const std::string& slotName, int groupNum=-1);
            void ConnectSignalToSlot(KTSignalWrapper* signal, KTSlotWrapper* slot, int groupNum=-1);

            template< class XProcessor >
            void RegisterSignal(std::string name, XProcessor* signalPtr);

            template< class XTarget, typename XReturn >
            void RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)());

            template< class XTarget, typename XReturn, typename XArg1 >
            void RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArg1));

            template< class XTarget, typename XReturn, typename XArg1, typename XArg2 >
            void RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArg1, XArg2));

            KTSignalWrapper* GetSignal(const std::string& name);

            KTSlotWrapper* GetSlot(const std::string& name);

    };


    template< typename XSignalSig >
    void KTProcessor::RegisterSignal(std::string name, XSignalSig* signalPtr)
    {
        KTDEBUG(processorlog, "Registering signal <" << name << "> in processor <" << fConfigName << ">");
        KTSignalWrapper* sig = new KTSignalWrapper(signalPtr);
        fSignalMap.insert(SigMapVal(name, sig));
        return;
    }

    template< class XTarget, typename XReturn >
    void KTProcessor::RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)())
    {
        KTDEBUG(processorlog, "Registering slot <" << name << "> in processor <" << fConfigName << ">");

        KTSignalConcept< XReturn () > signalConcept;

        boost::function< XReturn () > *func = new boost::function< XReturn () >(boost::bind(funcPtr, target));

        KTSlotWrapper* slot = new KTSlotWrapper(func, &signalConcept);
        fSlotMap.insert(SlotMapVal(name, slot));
        return;
    }

    template< class XTarget, typename XReturn, typename XArg1 >
    void KTProcessor::RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArg1))
    {
        KTDEBUG(processorlog, "Registering slot <" << name << "> in processor <" << fConfigName << ">");

        KTSignalConcept< XReturn (XArg1) > signalConcept;

        boost::function< XReturn (XArg1) > *func = new boost::function< XReturn (XArg1) >(boost::bind(funcPtr, target, _1));

        KTSlotWrapper* slot = new KTSlotWrapper(func, &signalConcept);
        fSlotMap.insert(SlotMapVal(name, slot));
        return;
    }

    template< class XTarget, typename XReturn, typename XArg1, typename XArg2 >
    void KTProcessor::RegisterSlot(std::string name, XTarget* target, XReturn (XTarget::* funcPtr)(XArg1, XArg2))
    {
        KTDEBUG(processorlog, "Registering slot <" << name << "> in processor <" << fConfigName << ">");

        KTSignalConcept< XReturn (XArg1, XArg2) > signalConcept;

        boost::function< XReturn (XArg1, XArg2) > *func = new boost::function< XReturn (XArg1, XArg2) >(boost::bind(funcPtr, target, _1, _2));

        KTSlotWrapper* slot = new KTSlotWrapper(func, &signalConcept);
        fSlotMap.insert(SlotMapVal(name, slot));
        return;
    }


#define KT_REGISTER_PROCESSOR(proc_class, proc_name) \
        static ::scarab::registrar< ::Nymph::KTProcessor, proc_class, const std::string& > sProc##proc_class##Registrar( proc_name );

}*/
#endif /* KTPROCESSORPY_HH_ */
