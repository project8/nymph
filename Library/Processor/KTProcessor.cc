/*
 * KTProcessor.cc
 *
 *  Created on: Jan 5, 2012
 *      Author: nsoblath
 */

#include "KTProcessor.hh"

//#include "KTLogger.hh"

#include <boost/foreach.hpp>

#include <string>

using std::string;

namespace Nymph
{
    //KTLOGGER(proclog, "KTProcessor");

    ProcessorException::ProcessorException (std::string const& why)
      : std::logic_error(why)
    {}


    KTProcessor::KTProcessor(const string& name) :
            KTConfigurable(name),
            fSignalMap(),
            fSlotMap(),
            fSlotToSigMap(),
            fSigConnMap()
    {
    }

    KTProcessor::~KTProcessor()
    {
        for (SlotMapIt iter = fSlotMap.begin(); iter != fSlotMap.end(); iter++)
        {
            iter->second->Disconnect();
            delete iter->second;
        }
        for (SigMapIt iter = fSignalMap.begin(); iter != fSignalMap.end(); iter++)
        {
            delete iter->second;
        }
    }

    void KTProcessor::PassThreadRefUpdate(const std::string& slotName, KTThreadReference* threadRef)
    {
        // get the list of slot-to-signal connections for this slot
        auto stsRange = fSlotToSigMap.equal_range(slotName);

        // loop over signals called in the performance of slot slotName
        for (SlotToSigMapCIt stsIt = stsRange.first; stsIt != stsRange.second; ++stsIt)
        {
            // loop over all processor:slots called by this signal
            auto sigConnRange = fSigConnMap.equal_range(stsIt->second);
            for (SigConnMapCIt sigConnIt = sigConnRange.first; sigConnIt != sigConnRange.second; ++sigConnIt)
            {
                // update the thread reference pointer for this connection
                sigConnIt->second.fThreadRef = threadRef;
                // pass the update on to the connected-to processor
                sigConnIt->second.fProc->PassThreadRefUpdate(sigConnIt->second.fSlotName, threadRef);
            }
        }
    }

    void KTProcessor::ConnectASlot(const std::string& signalName, KTProcessor* processor, const std::string& slotName, int groupNum)
    {
        // get the signal and slot wrapper pointers
        KTSignalWrapper* signal = GetSignal(signalName);
        KTSlotWrapper* slot = processor->GetSlot(slotName);

        try
        {
            // make the connection
            ConnectSignalToSlot(signal, slot, groupNum);
        }
        catch (std::exception& e)
        {
            string errorMsg = string("Exception caught in KTProcessor::ConnectASignal; signal: ") +
                    signalName + string(", slot: ") + slotName + string("\n") + e.what() + string("\n") +
                    string("\tIf the signal wrapper cannot be cast correctly, check that the signatures of the signal and slot match exactly.\n") +
                    string("\tIf the signal pointer is NULL, you may have the signal name wrong.\n") +
                    string("\tIf the slot pointer is NULL, you may have the slot name wrong.");
            throw std::logic_error(errorMsg);
        }

        // record the connection in the signal-connection map
        fSigConnMap.insert(SigConnMapVal(signalName, std::make_pair(processor, slotName)));

        KTDEBUG(processorlog, "Connected signal <" << this->GetConfigName() << ":" << signalName << "> to slot <" << processor->GetConfigName() << ":" << slotName << ">");

        return;
    }

    void KTProcessor::ConnectSignalToSlot(KTSignalWrapper* signal, KTSlotWrapper* slot, int groupNum)
    {
        if (signal == NULL)
        {
            throw ProcessorException("Signal pointer was NULL");
        }
        if (slot == NULL)
        {
            throw ProcessorException("Slot pointer was NULL");
        }

        slot->SetConnection(signal, groupNum);

        return;
    }

    KTSignalWrapper* KTProcessor::GetSignal(const std::string& name)
    {
        SigMapIt iter = fSignalMap.find(name);
        if (iter == fSignalMap.end())
        {
            return NULL;
        }
        return iter->second;
    }

    KTSlotWrapper* KTProcessor::GetSlot(const std::string& name)
    {
        SlotMapIt iter = fSlotMap.find(name);
        if (iter == fSlotMap.end())
        {
            return NULL;
        }
        return iter->second;
    }



} /* namespace Nymph */
