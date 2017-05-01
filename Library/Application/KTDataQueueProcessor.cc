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
        RegisterSlot("data", this, &KTDataQueueProcessor::QueueData);
        //RegisterSlot("data-list", this, &KTDataQueueProcessor::QueueDataList);
    }

    KTDataQueueProcessor::~KTDataQueueProcessor()
    {

    }

    bool KTDataQueueProcessor::ConfigureSubClass(const scarab::param_node*)
    {
        return true;
    }

    void KTDataQueueProcessor::EmitDataSignal(KTDataPtr data, KTDataPtrReturn& ret)
    {
        fDataSignal(data, ret);
        return;
    }

    void KTDataQueueProcessor::QueueData(KTDataPtr& data, KTDataPtrReturn& ret)
    {
        return DoQueueData(data, ret, &KTDataQueueProcessor::EmitDataSignal);
    }
/*
    void KTDataQueueProcessor::QueueDataList(list< KTDataPtr >* dataList)
    {
        return DoQueueDataList(dataList, &KTDataQueueProcessor::EmitDataSignal);
    }
*/
}
