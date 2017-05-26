/*
 * KTProcessor.cc
 *
 *  Created on: Jan 5, 2012
 *      Author: nsoblath
 */

#include "KTProcessor.hh"

#include "KTException.hh"
//#include "KTLogger.hh"

#include <string>

using std::string;

namespace Nymph
{
    //KTLOGGER(proclog, "KTProcessor");

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

    void KTProcessor::PassThreadRefUpdate(const std::string& slotName, std::shared_ptr< KTThreadReference > threadRef)
    {
        std::function< void(std::shared_ptr< KTThreadReference >) > funcObj = [this, &slotName](std::shared_ptr< KTThreadReference > ref){ GetSlot(slotName)->SetThreadRef(ref); };
        PassToConnProcs(slotName, funcObj, threadRef);
        return;
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
        catch( KTSignalException& e )
        {
            e << KTErrorMsgInfo< struct proc_Sig_0 >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem with the signal." );
            e << KTErrorMsgInfo< struct proc_Sig_1 >( "You may have the signal name wrong." );
            throw;
        }
        catch( KTSlotException& e )
        {
            e << KTErrorMsgInfo< struct proc_Slot_0 >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem with the slot." );
            e << KTErrorMsgInfo< struct proc_Slot_1 >( "You may have the slot name wrong." );
            throw;
        }
        catch( KTConnectionException& e )
        {
            e << KTErrorMsgInfo< struct proc_Conn_0 >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem making the connection." );
            e << KTErrorMsgInfo< struct proc_Conn_1 >( "Check that the signatures of the signal and slot match exactly." );
            throw;
        }
        catch( boost::exception& e )
        {
            e << KTErrorMsgInfo< struct proc_Unkn >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> for an unknown reason." );
            throw;
        }

        // record the connection in the signal-connection map
        fSigConnMap.insert(SigConnMapVal(signalName, std::make_pair(processor, slotName)));

        KTDEBUG(processorlog, "Connected signal <" << this->GetConfigName() << ":" << signalName << "> to slot <" << processor->GetConfigName() << ":" << slotName << ">");

        return;
    }

    void KTProcessor::ConnectSignalToSlot(KTSignalWrapper* signal, KTSlotWrapper* slot, int groupNum)
    {
        if (signal == nullptr)
        {
            BOOST_THROW_EXCEPTION( KTSignalException() << "Signal pointer was NULL" << eom );
        }
        if (slot == nullptr)
        {
            BOOST_THROW_EXCEPTION( KTSlotException() << "Slot pointer was NULL" << eom );
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

    bool KTProcessor::GetDoBreakpoint(const std::string& slotName)
    {
        KTSlotWrapper* slot = GetSlot(slotName);
        if (slot != nullptr)
        {
            return slot->GetDoBreakpoint();
        }
        BOOST_THROW_EXCEPTION( KTException() << "Slot <" << slotName << "> was not found" << eom );
        return false;
    }

    void KTProcessor::SetDoBreakpoint(const std::string& slotName, bool flag)
    {
        KTSlotWrapper* slot = GetSlot(slotName);
        if (slot != nullptr)
        {
            return slot->SetDoBreakpoint(flag);
        }
        BOOST_THROW_EXCEPTION( KTException() << "Slot <" << slotName << "> was not found" << eom );
        return;
    }

} /* namespace Nymph */
