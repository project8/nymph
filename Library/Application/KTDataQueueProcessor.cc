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

    bool KTDataQueueProcessor::ConfigureSubClass(const scarab::param_node*)
    {
        return true;
    }

    void KTDataQueueProcessor::EmitDataSignal(KTDataPtr data)
    {
        fDataSignal(data);
        return;
    }

    void KTDataQueueProcessor::QueueData(KTDataPtr& data)
    {
        fQueueDataSW->GetThreadRef()->Break(data);
        return DoQueueData(data);
    }
/*
    void KTDataQueueProcessor::QueueDataList(list< KTDataPtr >* dataList)
    {
        return DoQueueDataList(dataList, &KTDataQueueProcessor::EmitDataSignal);
    }
*/
}
