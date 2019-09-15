/*
 * Processor.cc
 *
 *  Created on: Jan 5, 2012
 *      Author: N.S. Oblath
 */

#include "Processor.hh"

#include <utility>

namespace Nymph
{
    //KTLOGGER(proclog, "Processor");

    Processor::Processor( const std::string& name ) :
            fName( name ),
            fSignals(),
            fSlots()
    {
    }

    Processor::~Processor()
    {}

    void Processor::RegisterSignal( std::string name, SignalBase* signal )
    {
        LDEBUG( processorlog, "Registering signal <" << name << "> in processor <" << fName << ">" );
        fSignals.insert( SigMapVal(name, signal) );

        // give the signal to any slots that are waiting for it
        auto range = fSlotsWaitingForSignals.equal_range( name );
        for( auto rangeIt = range.first; rangeIt != range.second; ++rangeIt )
        {
            rangeIt->second->SignalsUsed().push_back( signal );
        }

        return;
    }

    void Processor::RegisterSlot( std::string name, SlotBase* slot, std::initializer_list< std::string > signals )
    {
        LDEBUG( processorlog, "Registering slot <" << name << "> in processor <" << fName << ">" );
        fSlots.insert( SlotMapVal(name, slot) );

        // take care of giving signal pointers to the slot, or saving them for later assignment
        for( auto signalIt = signals.begin(); signalIt != signals.end(); ++signalIt )
        {
            auto signalPtrIt = fSignals.find( *signalIt );
            if( signalPtrIt == fSignals.end() )
            {
                // don't have that signal yet, so the slot will wait
                fSlotsWaitingForSignals.insert( std::make_pair( *signalIt, slot ) );
            }
            else
            {
                // have the signal; give it to the slot
                slot->SignalsUsed().push_back( signalPtrIt->second );
            }
        }

        return;
    }

    void Processor::ConnectASlot( const std::string& signalName, Processor& processor, const std::string& slotName, int groupNum )
    {
        // get the signal and slot pointers
        SigMapIt signalIt = fSignals.find(signalName);
        if( signalIt == fSignals.end() )
        {
            BOOST_THROW_EXCEPTION( SignalException() << "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem with the signal.\n" <<
                                                        "You may have the signal name wrong." << eom );
        }

        SlotMapIt slotIt = processor.fSlots.find(slotName);
        if( slotIt == processor.fSlots.end() )
        {
            BOOST_THROW_EXCEPTION( SlotException() << "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem with the slot." << 
                                                      "You may have the slot name wrong." << eom );
        }

        try
        {
            // make the connection
            ConnectSignalToSlot( signalIt->second, slotIt->second, groupNum );
        }
        catch( SignalException& e )
        {
            e << ErrorMsgInfo< struct proc_Sig_0 >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem with the signal." );
            e << ErrorMsgInfo< struct proc_Sig_1 >( "You may have the signal name wrong." );
            throw;
        }
        catch( SlotException& e )
        {
            e << ErrorMsgInfo< struct proc_Slot_0 >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem with the slot." );
            e << ErrorMsgInfo< struct proc_Slot_1 >( "You may have the slot name wrong." );
            throw;
        }
        catch( ConnectionException& e )
        {
            e << ErrorMsgInfo< struct proc_Conn_0 >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> due to a problem making the connection." );
            e << ErrorMsgInfo< struct proc_Conn_1 >( "Check that the signatures of the signal and slot match exactly." );
            throw;
        }
        catch( boost::exception& e )
        {
            e << ErrorMsgInfo< struct proc_Unkn >( "Unable to connect signal <" + signalName + "> to slot <" + slotName + "> for an unknown reason." );
            throw;
        }

        LDEBUG(processorlog, "Connected signal <" << this->Name() << ":" << signalName << "> to slot <" << processor.Name() << ":" << slotName << ">");

        return;
    }

    void Processor::ConnectSignalToSlot( SignalBase* signal, SlotBase* slot, int groupNum )
    {
        if( ! signal )
        {
            BOOST_THROW_EXCEPTION( SignalException() << "Signal pointer was NULL" << eom );
        }
        if( ! slot )
        {
            BOOST_THROW_EXCEPTION( SlotException() << "Slot pointer was NULL" << eom );
        }

        signal->Connect(slot, groupNum);

        return;
    }
/*
    bool Processor::GetDoBreakpoint( const std::string& slotName )
    {
        KTSlotBase* slot = GetSlot(slotName);
        if (slot != nullptr)
        {
            return slot->GetDoBreakpoint();
        }
        BOOST_THROW_EXCEPTION( KTException() << "Slot <" << slotName << "> was not found" << eom );
        return false;
    }

    void Processor::SetDoBreakpoint( const std::string& slotName, bool flag )
    {
        KTSlotBase* slot = GetSlot(slotName);
        if (slot != nullptr)
        {
            return slot->SetDoBreakpoint(flag);
        }
        BOOST_THROW_EXCEPTION( KTException() << "Slot <" << slotName << "> was not found" << eom );
        return;
    }
*/
} /* namespace Nymph */
