/*
 * KTProcessorToolbox.cc
 *
 *  Created on: Sep 27, 2012
 *      Author: nsoblath
 */

#include "KTProcessorToolbox.hh"

#include "KTLogger.hh"
#include "KTPrimaryProcessor.hh"

#include "factory.hh"
#include "param_codec.hh"

#include <vector>

#ifndef SINGLETHREADED
#include <boost/thread.hpp>
#endif

using std::deque;
using std::set;
using std::string;
using std::vector;

namespace Nymph
{
    KTLOGGER(proclog, "KTProcessorToolbox");

    KTProcessorToolbox::KTProcessorToolbox(const std::string& name) :
            KTConfigurable(name),
            fRunQueue(),
            fProcMap()
    {
    }

    KTProcessorToolbox::~KTProcessorToolbox()
    {
        ClearProcessors();
    }

    bool KTProcessorToolbox::Configure(const scarab::param_node& node)
    {
        auto tProcFactory = scarab::factory< KTProcessor, const std::string& >::get_instance();

        KTPROG(proclog, "Configuring . . .");
        // Deal with "processor" blocks first
        if (! node.has("processors"))
        {
            KTWARN(proclog, "No processors were specified");
        }
        else
        {
            const scarab::param_array& procArray = node["processors"].as_array();
            for( scarab::param_array::const_iterator procIt = procArray.begin(); procIt != procArray.end(); ++procIt )
            {
                if( ! procIt->is_node() )
                {
                    KTERROR( proclog, "Invalid processor entry: not a node" );
                    return false;
                }
                const scarab::param_node& procNode = procIt->as_node();

                if (! procNode.has("type"))
                {
                    KTERROR(proclog, "Unable to create processor: no processor type given");
                    return false;
                }
                string procType = procNode["type"]().as_string();

                string procName;
                if (! procNode.has("name"))
                {
                    KTINFO(proclog, "No name given for processor of type <" << procType << ">; using type as name.");
                    procName = procType;
                }
                else
                {
                    procName = procNode["name"]().as_string();
                }
                KTProcessor* newProc = tProcFactory->create(procType, procType);
                if (newProc == NULL)
                {
                    KTERROR(proclog, "Unable to create processor of type <" << procType << ">");
                    return false;
                }

                if (! AddProcessor(procName, newProc))
                {
                    KTERROR(proclog, "Unable to add processor <" << procName << ">");
                    delete newProc;
                    return false;
                }
            }
        }


        // Then deal with connections"
        if (! node.has("connections"))
        {
            KTWARN(proclog, "No connections were specified");
        }
        else
        {
            const scarab::param_array& connArray = node["connections"].as_array();
            for( scarab::param_array::const_iterator connIt = connArray.begin(); connIt != connArray.end(); ++connIt )
            {
                if( ! connIt->is_node() )
                {
                    KTERROR( proclog, "Invalid connection entry: not a node" );
                    return false;
                }
                const scarab::param_node& connNode = connIt->as_node();

                if ( ! connNode.has("signal") || ! connNode.has("slot") )
                {
                    KTERROR(proclog, "Signal/Slot connection information is incomplete!");
                    if (connNode.has("signal"))
                    {
                        KTWARN(proclog, "signal = " << connNode["signal"]());
                    }
                    else
                    {
                        KTERROR(proclog, "signal = MISSING");
                    }

                    if (connNode.has("slot"))
                    {
                        KTWARN(proclog, "slot = " << connNode["slot"]());
                    }
                    else
                    {
                        KTERROR(proclog, "slot = MISSING");
                    }
                    return false;
                }

                bool connReturn = false;
                if (connNode.has("order"))
                {
                    connReturn = MakeConnection(connNode["signal"]().as_string(), connNode["slot"]().as_string(), connNode["order"]().as_int());
                }
                else
                {
                    connReturn = MakeConnection(connNode["signal"]().as_string(), connNode["slot"]().as_string());
                }
                if (! connReturn)
                {
                    KTERROR(proclog, "Unable to make connection <" << connNode["signal"]().as_string() << "> --> <" << connNode["slot"]().as_string() << ">");
                    return false;
                }

                KTINFO(proclog, "Signal <" << connNode["signal"]().as_string() << "> connected to slot <" << connNode["slot"]().as_string() << ">");
            }
        }


        // Finally, deal with processor-run specifications
        // The run queue is an array of processor names, or groups of names, which will be run sequentially.
        // If names are grouped (in another array), those in that group will be run in parallel.
        // In single threaded mode all threads will be run sequentially in the order they were specified.
        if (! node.has("run-queue") || ! node["run-queue"].is_array())
        {
            KTWARN(proclog, "Run queue was not specified");
        }
        else
        {
            const scarab::param_array& rqArray = node["run-queue"].as_array();
            for (scarab::param_array::const_iterator rqIt = rqArray.begin(); rqIt != rqArray.end(); ++rqIt)
            {
                if (rqIt->is_value())
                {
                    if (! PushBackToRunQueue((*rqIt)().as_string()))
                    {
                        KTERROR(proclog, "Unable to process run-queue entry: could not add processor to the queue");
                        return false;
                    }
                }
                else if (rqIt->is_array())
                {
                    const scarab::param_array& rqNode = rqIt->as_array();
                    std::vector< std::string > names;

                    for (scarab::param_array::const_iterator rqArrayIt = rqNode.begin(); rqArrayIt != rqNode.end(); ++rqArrayIt)
                    {
                        if (! rqArrayIt->is_value())
                        {
                            KTERROR(proclog, "Invalid run-queue array entry: not a value");
                            return false;
                        }
                        names.push_back((*rqArrayIt)().as_string());
                    }

                    if (! PushBackToRunQueue(names))
                    {
                        KTERROR(proclog, "Unable to process run-queue entry: could not add list of processors to the queue");
                        return false;
                    }
                }
                else
                {
                    KTERROR(proclog, "Invalid run-queue entry: not a value or array");
                    return false;
                }
            }
        }

        return true;
    }

    bool KTProcessorToolbox::ConfigureProcessors(const scarab::param_node& node)
    {
        for (ProcMapIt iter = fProcMap.begin(); iter != fProcMap.end(); iter++)
        {
            KTDEBUG(proclog, "Attempting to configure processor <" << iter->first << ">");
            string procName = iter->first;
            string nameUsed;
            if (node.has(procName))
            {
                nameUsed = procName;
            }
            else
            {
                string configName = iter->second.fProc->GetConfigName();
                KTWARN(proclog, "Did not find a parameter node <" << procName << ">\n"
                        "\tWill check using the generic name of the processor, <" << configName << ">.");
                if (node.has(configName))
                {
                    nameUsed = configName;
                    KTWARN(proclog, "Configuring processor <" << procName << "> with configuration found using the generic name of the processor: <" << configName << ">");
                }
                else
                {
                    KTWARN(proclog, "Did not find configuration information for processor <" << procName << "> (type <" << configName << ">)\n"
                            "\tProcessor <" << procName << "> was not configured.");
                    continue;
                }
            }

            const scarab::param_node& subNode = node[nameUsed].as_node();

            if (! iter->second.fProc->Configure(subNode))
            {
                KTERROR(proclog, "An error occurred while configuring processor <" << iter->first << "> with parameter node <" << nameUsed << ">");
                return false;
            }
        }
        return true;
    }

    bool KTProcessorToolbox::ConfigureProcessors(const std::string& config)
    {
        scarab::param_translator translator;
        scarab::param_node optNode;
        optNode.add( "encoding", new scarab::param_value( "json" ) );
        return ConfigureProcessors( translator.read_string( config, optNode )->as_node() );
    }

    bool KTProcessorToolbox::Run()
    {
        KTPROG(proclog, "Beginning processing . . .");
#ifndef SINGLETHREADED
        unsigned iGroup = 0;
#endif
        for (RunQueue::const_iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
        {
#ifdef SINGLETHREADED
            for (ThreadGroup::const_iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
            {
                if (! tgIter->fProc->Run())
                {
                    return false;
                }
            }
#else
            KTDEBUG(proclog, "Starting thread group " << iGroup);
            boost::thread_group parallelThreads;
            unsigned iThread = 0;
            for (ThreadGroup::const_iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
            {
                // create a boost::thread object to launch the thread
                // use boost::ref to avoid copying the processor
                KTDEBUG(proclog, "Starting thread " << iThread << ": " << tgIter->fName);
                parallelThreads.create_thread(boost::ref(*(tgIter->fProc)));
                //parallelThreads.create_thread(boost::ref(**tgIter));
                iThread++;
            }
            // wait for execution to complete
            parallelThreads.join_all();
            iGroup++;
#endif
        }
        KTPROG(proclog, ". . . processing complete.");
        return true;
    }


    KTProcessor* KTProcessorToolbox::GetProcessor(const std::string& procName)
    {
        ProcMapIt it = fProcMap.find(procName);
        if (it == fProcMap.end())
        {
            KTWARN(proclog, "Processor <" << procName << "> was not found.");
            return NULL;
        }
        return it->second.fProc;
    }

    const KTProcessor* KTProcessorToolbox::GetProcessor(const std::string& procName) const
    {
        ProcMapCIt it = fProcMap.find(procName);
        if (it == fProcMap.end())
        {
            KTWARN(proclog, "Processor <" << procName << "> was not found.");
            return NULL;
        }
        return it->second.fProc;
    }

    bool KTProcessorToolbox::AddProcessor(const std::string& procName, KTProcessor* proc)
    {
        ProcMapIt it = fProcMap.find(procName);
        if (it == fProcMap.end())
        {
            ProcessorInfo pInfo;
            pInfo.fProc = proc;
            fProcMap.insert(ProcMapValue(procName, pInfo));
            KTDEBUG(proclog, "Added processor <" << procName << "> (a.k.a. " << proc->GetConfigName() << ")");
            return true;
        }
        KTWARN(proclog, "Processor <" << procName << "> already exists; new processor was not added.");
        return false;
    }

    bool KTProcessorToolbox::AddProcessor(const std::string& procType, const std::string& procName)
    {
        auto tProcFactory = scarab::factory< KTProcessor, const std::string& >::get_instance();

        ProcMapIt it = fProcMap.find(procName);
        if (it == fProcMap.end())
        {
            KTProcessor* newProc = tProcFactory->create(procType, procType);
            if (newProc == NULL)
            {
                KTERROR(proclog, "Unable to create processor of type <" << procType << ">");
                return false;
            }
            if (! AddProcessor(procName, newProc))
            {
                KTERROR(proclog, "Unable to add processor <" << procName << ">");
                delete newProc;
                return false;
            }
            return true;
        }
        KTWARN(proclog, "Processor <" << procName << "> already exists; new processor was not added.");
        return false;
    }

    bool KTProcessorToolbox::RemoveProcessor(const std::string& procName)
    {
        KTProcessor* procToRemove = ReleaseProcessor(procName);
        if (procToRemove == NULL)
        {
            return false;
        }
        delete procToRemove;
        KTDEBUG(proclog, "Processor <" << procName << "> deleted.");
        return true;
    }

    KTProcessor* KTProcessorToolbox::ReleaseProcessor(const std::string& procName)
    {
        ProcMapIt it = fProcMap.find(procName);
        if (it == fProcMap.end())
        {
            KTWARN(proclog, "Processor <" << procName << "> was not found.");
            return NULL;
        }
        KTProcessor* procToRelease = it->second.fProc;
        fProcMap.erase(it);
        return procToRelease;
    }

    void KTProcessorToolbox::ClearProcessors()
    {
        for (ProcMapIt it = fProcMap.begin(); it != fProcMap.end(); it++)
        {
            delete it->second.fProc;
        }
        fProcMap.clear();
        fRunQueue.clear();
        return;
    }


    bool KTProcessorToolbox::MakeConnection(const std::string& signal, const std::string& slot, int order)
    {
        string signalProcName, signalName;
        if (! ParseSignalSlotName(signal, signalProcName, signalName))
        {
            KTERROR(proclog, "Unable to parse signal name: <" << signal << ">");
            return false;
        }

        string slotProcName, slotName;
        if (! ParseSignalSlotName(slot, slotProcName, slotName))
        {
            KTERROR(proclog, "Unable to parse slot name: <" << slot << ">");
            return false;
        }

        return MakeConnection(signalProcName, signalName, slotProcName, slotName, order);
    }

    bool KTProcessorToolbox::MakeConnection(const std::string& signalProcName, const std::string& signalName, const std::string& slotProcName, const std::string& slotName, int order)
    {
        KTProcessor* signalProc = GetProcessor(signalProcName);
        if (signalProc == NULL)
        {
            KTERROR(proclog, "Processor named <" << signalProcName << "> was not found!");
            return false;
        }

        KTProcessor* slotProc = GetProcessor(slotProcName);
        if (slotProc == NULL)
        {
            KTERROR(proclog, "Processor named <" << slotProcName << "> was not found!");
            return false;
        }

        try
        {
            if (order != std::numeric_limits< int >::min())
            {
                signalProc->ConnectASlot(signalName, slotProc, slotName, order);
            }
            else
            {
                signalProc->ConnectASlot(signalName, slotProc, slotName);
            }
        }
        catch (std::exception& e)
        {
            KTERROR(proclog, "An error occurred while connecting signals and slots:\n"
                    << "\tSignal " << signalName << " from processor " << signalProcName << " (a.k.a. " << signalProc->GetConfigName() << ")" << '\n'
                    << "\tSlot " << slotName << " from processor " << slotProcName << " (a.k.a. " << slotProc->GetConfigName() << ")" << '\n'
                    << '\t' << e.what());
            return false;
        }

        return true;
    }

    bool KTProcessorToolbox::ParseSignalSlotName(const std::string& toParse, std::string& nameOfProc, std::string& nameOfSigSlot) const
    {
        size_t sepPos = toParse.find_first_of(fSigSlotNameSep);
        if (sepPos == string::npos)
        {
            KTERROR(proclog, "Unable to find separator between processor and signal/slot name in <" << toParse << ">");
            return false;
        }
        nameOfProc = toParse.substr(0, sepPos);
        nameOfSigSlot = toParse.substr(sepPos + 1);
        return true;
    }


    bool KTProcessorToolbox::PushBackToRunQueue(const std::string& name)
    {
        ThreadGroup threadGroup;

        if (! AddProcessorToThreadGroup( name, threadGroup))
        {
            KTERROR(proclog, "Unable to add processor <" << name << "> to thread group");
            return false;
        }

        fRunQueue.push_back(threadGroup);

        KTINFO(proclog, "Added processor <" << name << "> to the run queue");
        return true;
    }

    bool KTProcessorToolbox::PushBackToRunQueue(std::initializer_list< std::string > names)
    {
        return PushBackToRunQueue(std::vector< std::string >(names));
    }

    bool KTProcessorToolbox::PushBackToRunQueue(std::vector< std::string > names)
    {
        ThreadGroup threadGroup;

        std::stringstream toPrint;
        for (const std::string& name : names)
        {
            if (! AddProcessorToThreadGroup(name, threadGroup))
            {
                KTERROR(proclog, "Unable to add processor <" << name << "> to thread group");
                return false;
            }
            toPrint << name << ", "; // the extra comma at the end is removed below
        }

        fRunQueue.push_back(threadGroup);
        std::string toPrintString = toPrint.str();
        toPrintString.resize(toPrintString.size()-2);
        KTINFO(proclog, "Added processors <" << toPrintString << "> to the run queue");
        return true;
    }

    bool KTProcessorToolbox::AddProcessorToThreadGroup(const std::string& name, ThreadGroup& group)
    {
        KTProcessor* procForRunQueue = GetProcessor(name);
        KTDEBUG(proclog, "Attempting to add processor <" << name << "> to the run queue");
        if (procForRunQueue == NULL)
        {
            KTERROR(proclog, "Unable to find processor <" << name << "> requested for the run queue");
            return false;
        }

        KTPrimaryProcessor* primaryProc = dynamic_cast< KTPrimaryProcessor* >(procForRunQueue);
        if (primaryProc == NULL)
        {
            KTERROR(proclog, "Processor <" << name << "> is not a primary processor.");
            return false;
        }
        //group.insert(primaryProc);
        group.insert(Thread(primaryProc, name));
        return true;
    }

} /* namespace Nymph */
