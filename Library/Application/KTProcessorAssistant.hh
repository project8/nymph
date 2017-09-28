/*
 * KTProcessorAssistant.hh
 *
 *  Created on: Sep 23, 2017
 *      Author: N.S. Oblath
 */

#include "KTProcessor.hh"

#include "KTSlot.hh"

#ifndef NYMPH_KTPROCESSORASSISTANT_HH_
#define NYMPH_KTPROCESSORASSISTANT_HH_

namespace Nymph
{


    template< class XCallable >
    class KTProcessorAssistant
    {
        public:
            KTProcessorAssistant(const std::string& name, XCallable& callable);
            virtual ~KTProcessorAssistant();

            bool Configure(const scarab::param_node& node);

            void ExecuteCallable(KTDataHandle dataHandle);

        private:
            XCallable& fCallable;

            KTSignalData fSignal;

            KTSlotData fSlot;
    };

    template< class XCallable >
    KTProcessorAssistant< XCallable >::KTProcessorAssistant(const std::string& name, XCallable& callable) :
            KTProcessor(name),
            fCallable(callable),
            fSignal("signal", this),
            fSlot("slot", this, &KTProcessorAssistant::ExecuteCallable, {"signal"})
    {
    }

    template< class XCallable >
    KTProcessorAssistant< XCallable >::~KTProcessorAssistant()
    {
    }

    template< class XCallable >
    bool KTProcessorAssistant< XCallable >::Configure(const scarab::param_node& node)
    {
        return true;
    }

    template< class XCallable >
    void KTProcessorAssistant< XCallable >::ExecuteCallable(KTDataHandle dataHandle)
    {
        fCallable(dataHandle);
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_KTPROCESSORASSISTANT_HH_ */
