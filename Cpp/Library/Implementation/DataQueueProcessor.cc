/*
 * DataQueueProcessor.cc
 *
 *  Created on: Feb 5, 2013
 *      Author: nsoblath
 * modified for nymph2 by Yuhao Sun 2022 06 15
 */

#include "DataQueueProcessor.hh"

//using std::list;

namespace Nymph
{
    REGISTER_PROCESSOR(DataQueueProcessor, "data-queue");

    DataQueueProcessor::DataQueueProcessor(const std::string& name) :
        DataQueueProcessorTemplate< DataQueueProcessor >(name),
        fDataSignal("data", this)
    {
        SetFuncPtr(&DataQueueProcessor::EmitDataSignal);
        fQueueDataSW = RegisterSlot("data", this, &DataQueueProcessor::QueueData, {});
        fSignalsEmitted.push_back("data");
        //RegisterSlot("data-list", this, &DataQueueProcessor::QueueDataList);
    }

    DataQueueProcessor::~DataQueueProcessor()
    {

    }

    bool DataQueueProcessor::ConfigureSubClass(const scarab::param_node&)
    {
        return true;
    }

    void DataQueueProcessor::EmitDataSignal(DataHandle data)
    {
        fDataSignal(data);
        return;
    }

    void DataQueueProcessor::QueueData(DataHandle& data)
    {
        fQueueDataSW->GetThreadRef()->Break(data, fQueueDataSW->GetDoBreakpoint());
        return DoQueueData(data);
    }
/*
    void DataQueueProcessor::QueueDataList(list< DataHandle >* dataList)
    {
        return DoQueueDataList(dataList, &DataQueueProcessor::EmitDataSignal);
    }
*/
}
