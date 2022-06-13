/*
 * ProcessorToolbox.cc
 *
 *  Created on: Sep 27, 2012
 *      Author: N.S. Oblath
 */

#include "ProcessorToolbox.hh"

#include "PrimaryProcessor.hh"

#include "factory.hh"
#include "logger.hh"
#include "param_codec.hh"

#include <map>

using std::string;

namespace Nymph
{
    LOGGER(proclog, "ProcessorToolbox");

    ProcessorToolbox::ProcessorToolbox( const std::string& name ) :
            fProcFactory( scarab::factory< Processor, const std::string& >::get_instance() ),
            fProcMap()
    {
    }

    ProcessorToolbox::~ProcessorToolbox()
    {}

    void ProcessorToolbox::Configure( const scarab::param_node& node )
    {
        LPROG( proclog, "Configuring processor toolbox" );

        // Deal with "processor" blocks
        if( ! node.has("processors") )
        {
            LWARN( proclog, "No processors were specified" );
        }
        else
        {
            ConfigureProcessors( node["processors"].as_array() );
        }

        // Then deal with connections"
        if( ! node.has("connections") )
        {
            LWARN( proclog, "No connections were specified" );
        }
        else
        {
            ConfigureConnections( node["connections"].as_array() );
        }

        return;
    }


    void ProcessorToolbox::ConfigureProcessors( const scarab::param_array& array )
    {
        for( auto procIt = array.begin(); procIt != array.end(); ++procIt )
        {
            if( ! procIt->is_node() )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Invalid processor entry (not a node): " << *procIt );
            }
            const scarab::param_node& procNode = procIt->as_node();

            if( ! procNode.has("type") )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Unable to create processor: no processor type given" );
            }
            string procType = procNode["type"]().as_string();

            string procName;
            if( ! procNode.has("name") )
            {
                LINFO(proclog, "No name given for processor of type <" << procType << ">; using type as name.");
                procName = procType;
            }
            else
            {
                procName = procNode["name"]().as_string();
            }

            std::shared_ptr< Processor > newProc ( fProcFactory->create(procType, procName) );
            if( newProc == nullptr )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Unable to create processor of type <" << procType << ">" );
            }

            LDEBUG( proclog, "Attempting to configure processor <" << procName << ">" );
            try
            {
                newProc->Configure(procNode);
            }
            catch( scarab::base_exception& e )
            {
                THROW_NESTED_EXCEPT_HERE( Exception() << "An error occurred while configuring processor <" << procName << ">" );
            }

            if( ! AddProcessor( procName, newProc ) )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Unable to add processor <" << procName << ">" );
            }
        }

        return;
    }


    void ProcessorToolbox::ConfigureConnections( const scarab::param_array& array )
    {
        for( auto connIt = array.begin(); connIt != array.end(); ++connIt )
        {
            if( ! connIt->is_node() )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Invalid connection entry: not a node" << *connIt );
            }
            const scarab::param_node& connNode = connIt->as_node();

            if( ! connNode.has("signal") || ! connNode.has("slot") )
            {
                std::string sigSlotMessage( "signal = " );
                if (connNode.has("signal"))
                {
                    sigSlotMessage += connNode["signal"]().as_string();
                }
                else
                {
                    sigSlotMessage += "MISSING";
                }

                sigSlotMessage += "\nslot = ";
                if (connNode.has("slot"))
                {
                    sigSlotMessage += connNode["slot"]().as_string();
                }
                else
                {
                    sigSlotMessage += "MISSING";
                }
                THROW_EXCEPT_HERE( ConfigException(array) << "Signal/Slot connection information is incomplete!\n" << sigSlotMessage );
            }

            bool connReturn = false;
            if( connNode.has("order") )
            {
                connReturn = MakeConnection( connNode["signal"]().as_string(), connNode["slot"]().as_string(), connNode["order"]().as_int() );
            }
            else
            {
                connReturn = MakeConnection( connNode["signal"]().as_string(), connNode["slot"]().as_string() );
            }
            if( ! connReturn )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Unable to make connection <" << connNode["signal"]() << "> --> <" << connNode["slot"]() << ">" );
            }

            if( connNode.has("breakpoint") )
            {
                if (! SetBreakpoint( connNode["slot"]().as_string() ) )
                {
                    THROW_EXCEPT_HERE( ConfigException(array) << "Unable to set breakpoint on <" << connNode["slot"]() );
                }
            }

            LINFO( proclog, "Signal <" << connNode["signal"]() << "> connected to slot <" << connNode["slot"]() << ">" );
        }

        return;
    }

    std::shared_ptr< Processor > ProcessorToolbox::GetProcessor( const std::string& procName )
    {
        ProcMapIt it = fProcMap.find( procName );
        if( it == fProcMap.end() )
        {
            LWARN( proclog, "Processor <" << procName << "> was not found." );
            return nullptr;
        }
        return it->second.fProc;
    }

    const std::shared_ptr< Processor > ProcessorToolbox::GetProcessor( const std::string& procName ) const
    {
        ProcMapCIt it = fProcMap.find( procName );
        if (it == fProcMap.end())
        {
            LWARN( proclog, "Processor <" << procName << "> was not found." );
            return nullptr;
        }
        return it->second.fProc;
    }

    bool ProcessorToolbox::AddProcessor( const std::string& procName, std::shared_ptr< Processor > proc )
    {
        ProcMapIt it = fProcMap.find( procName);
        if( it == fProcMap.end() )
        {
            ProcessorInfo pInfo;
            pInfo.fProc = proc;
            fProcMap.insert( ProcMapValue(procName, pInfo) );
            LDEBUG( proclog, "Added processor <" << procName << "> (a.k.a. " << proc->Name() << ")" );
            return true;
        }
        LWARN( proclog, "Processor <" << procName << "> already exists; new processor was not added." );
        return false;
    }

    bool ProcessorToolbox::AddProcessor( const std::string& procType, const std::string& procName )
    {
        ProcMapIt it = fProcMap.find( procName );
        if( it == fProcMap.end() )
        {
            std::shared_ptr< Processor > newProc ( fProcFactory->create(procType, procType) );
            if( newProc == nullptr )
            {
                LERROR( proclog, "Unable to create processor of type <" << procType << ">" );
                return false;
            }
            if( ! AddProcessor(procName, newProc) )
            {
                LERROR( proclog, "Unable to add processor <" << procName << ">" );
                return false;
            }
            return true;
        }
        LWARN( proclog, "Processor <" << procName << "> already exists; new processor was not added." );
        return false;
    }

    bool ProcessorToolbox::RemoveProcessor( const std::string& procName )
    {
        std::shared_ptr< Processor > procToRemove = ReleaseProcessor( procName );
        if( procToRemove == nullptr )
        {
            return false;
        }
        LDEBUG( proclog, "Processor <" << procName << "> deleted." );
        return true;
    }

    std::shared_ptr< Processor > ProcessorToolbox::ReleaseProcessor( const std::string& procName )
    {
        ProcMapIt it = fProcMap.find( procName );
        if( it == fProcMap.end() )
        {
            LWARN( proclog, "Processor <" << procName << "> was not found." );
            return nullptr;
        }
        std::shared_ptr< Processor > procToRelease = it->second.fProc;
        fProcMap.erase( it );
        return procToRelease;
    }

    void ProcessorToolbox::ClearProcessors()
    {
        fProcMap.clear();
        return;
    }


    bool ProcessorToolbox::MakeConnection( const std::string& signal, const std::string& slot, int order )
    {
        string signalProcName, signalName;
        if( ! ParseSignalSlotName( signal, signalProcName, signalName ) )
        {
            LERROR( proclog, "Unable to parse signal name: <" << signal << ">" );
            return false;
        }

        string slotProcName, slotName;
        if( ! ParseSignalSlotName( slot, slotProcName, slotName ) )
        {
            LERROR( proclog, "Unable to parse slot name: <" << slot << ">" );
            return false;
        }

        return MakeConnection( signalProcName, signalName, slotProcName, slotName, order );
    }

    bool ProcessorToolbox::MakeConnection( const std::string& signalProcName, const std::string& signalName, const std::string& slotProcName, const std::string& slotName, int order )
    {
        std::shared_ptr< Processor > signalProc = GetProcessor( signalProcName );
        if( signalProc == nullptr )
        {
            LERROR(proclog, "Processor named <" << signalProcName << "> was not found!");
            return false;
        }

        std::shared_ptr< Processor > slotProc = GetProcessor( slotProcName );
        if( slotProc == nullptr )
        {
            LERROR( proclog, "Processor named <" << slotProcName << "> was not found!" );
            return false;
        }

        try
        {
            if( order != std::numeric_limits< int >::min() )
            {
                signalProc->ConnectASlot( signalName, *slotProc.get(), slotName, order );
            }
            else
            {
                signalProc->ConnectASlot(signalName, *slotProc.get(), slotName);
            }
        }
        catch( scarab::base_exception& e )
        {
            LERROR( proclog, "An error occurred while connecting signals and slots:\n"
                    << "\tSignal " << signalName << " from processor " << signalProcName << " (a.k.a. " << signalProc->Name() << ")" << '\n'
                    << "\tSlot " << slotName << " from processor " << slotProcName << " (a.k.a. " << slotProc->Name() << ")" << '\n' );
            PrintException( e );
            return false;
        }

        return true;
    }

    bool ProcessorToolbox::SetBreakpoint( const std::string& signal )
    {
        string signalProcName, signalName;
        if(! ParseSignalSlotName( signal, signalProcName, signalName ) )
        {
            LERROR(proclog, "Unable to parse signal name: <" << signal << ">");
            return false;
        }

        return SetBreakpoint( signalProcName, signalName );
    }

    bool ProcessorToolbox::SetBreakpoint( const std::string& signalProcName, const std::string& signalName )
    {
        std::shared_ptr< Processor > signalProc = GetProcessor( signalProcName );
        if( signalProc == nullptr )
        {
            LERROR( proclog, "Processor named <" << signalProcName << "> was not found!" );
            return false;
        }

        try
        {
            signalProc->SetDoBreakpoint( signalName, true );
            return true;
        }
        catch( scarab::base_exception& e )
        {
            LERROR( proclog, "Unable to set breakpoint: " );
            PrintException( e );
            return false;
        }
    }

    bool ProcessorToolbox::RemoveBreakpoint( const std::string& signal )
    {
        string signalProcName, signalName;
        if( ! ParseSignalSlotName( signal, signalProcName, signalName ) )
        {
            LERROR( proclog, "Unable to parse signal name: <" << signal << ">" );
            return false;
        }

        return RemoveBreakpoint( signalProcName, signalName );
    }

    bool ProcessorToolbox::RemoveBreakpoint( const std::string& signalProcName, const std::string& signalName )
    {
        std::shared_ptr< Processor > signalProc = GetProcessor( signalProcName );
        if( signalProc == nullptr )
        {
            LERROR(proclog, "Processor named <" << signalProcName << "> was not found!");
            return false;
        }

        try
        {
            signalProc->SetDoBreakpoint( signalName, false );
            return true;
        }
        catch( scarab::base_exception& e )
        {
            LERROR( proclog, "Unable to set breakpoint: " );
            PrintException( e );
            return false;
        }
    }

    bool ProcessorToolbox::ParseSignalSlotName( const std::string& toParse, std::string& nameOfProc, std::string& nameOfSigSlot ) const
    {
        size_t sepPos = toParse.find_first_of( fSigSlotNameSep );
        if( sepPos == string::npos )
        {
            LERROR( proclog, "Unable to find separator between processor and signal/slot name in <" << toParse << ">" );
            return false;
        }
        nameOfProc = toParse.substr( 0, sepPos );
        nameOfSigSlot = toParse.substr( sepPos + 1 );
        return true;
    }

} /* namespace Nymph */
