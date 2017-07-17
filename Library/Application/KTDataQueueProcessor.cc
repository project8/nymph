/*
 * KTDataQueueProcessor.cc
 *
 *  Created on: Feb 5, 2013
 *      Author: nsoblath
 */

#include "KTDataQueueProcessor.hh"

//using std::list;

namespace Nymph
{
    KT_REGISTER_PROCESSOR(KTDataQueueProcessor, "data-queue");

    KTDataQueueProcessor::KTDataQueueProcessor(const std::string& name) :
        KTDataQueueProcessorTemplate< KTDataQueueProcessor >(name),
        fDataSignal("data", this)
    {
        SetFuncPtr(&KTDataQueueProcessor::EmitDataSignal);
        fQueueDataSW = RegisterSlot("data", this, &KTDataQueueProcessor::QueueData, {});
        fSignalsEmitted.push_back("data");
        //RegisterSlot("data-list", this, &KTDataQueueProcessor::QueueDataList);
    }

    KTDataQueueProcessor::~KTDataQueueProcessor()
    {

    }

    bool KTDataQueueProcessor::ConfigureSubClass(const scarab::param_node&)
    {
        return true;
    }

    void KTDataQueueProcessor::EmitDataSignal(KTDataHandle data)
    {
        fDataSignal(data);
        return;
    }

    void KTDataQueueProcessor::QueueData(KTDataHandle& data)
    {
        fQueueDataSW->GetThreadRef()->Break(data, fQueueDataSW->GetDoBreakpoint());
        return DoQueueData(data);
    }
/*
    void KTDataQueueProcessor::QueueDataList(list< KTDataHandle >* dataList)
    {
        return DoQueueDataList(dataList, &KTDataQueueProcessor::EmitDataSignal);
    }
*/
}
