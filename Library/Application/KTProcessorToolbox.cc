/*
 * KTProcessorToolbox.cc
 *
 *  Created on: Sep 27, 2012
 *      Author: nsoblath
 */

#include "KTProcessorToolbox.hh"

#include "KTException.hh"
#include "KTLogger.hh"
#include "KTPrimaryProcessor.hh"

#include "factory.hh"

#ifndef SINGLETHREADED
#include <boost/thread.hpp>
#endif

#include <future>
#include <thread>
#include <vector>

using std::deque;
using std::set;
using std::string;
using std::vector;

namespace Nymph
{
    KTLOGGER(proclog, "KTProcessorToolbox");

    KTProcessorToolbox::KTProcessorToolbox(const std::string& name) :
            KTConfigurable(name),
            fProcFactory(scarab::factory< KTProcessor, const std::string& >::get_instance()),
            fRunSingleThreaded( true ), //TODO: switch this to false once multithreading is available
            fRunQueue(),
            fProcMap(),
            fThreadFutures(),
            //fThreadPackets(),
            fContinueSignaler(),
            fMasterContSignal(),
            fBreakContMutex(),
            fDoRunThread( nullptr ),
            fDoRunPromise(),
            fDoRunBreakFlag( false )
    {
    }

    KTProcessorToolbox::~KTProcessorToolbox()
    {
        ClearProcessors();
    }

    bool KTProcessorToolbox::Configure(const scarab::param_node* node)
    {
        KTPROG(proclog, "Configuring . . .");

        SetRunSingleThreaded( node->get_value( "single-threaded", fRunSingleThreaded ) );

        // Deal with "processor" blocks
        const scarab::param_array* procArray = node->array_at( "processors" );
        if (procArray == NULL)
        {
            KTWARN(proclog, "No processors were specified");
        }
        else
        {
            for( scarab::param_array::const_iterator procIt = procArray->begin(); procIt != procArray->end(); ++procIt )
            {
                if( ! (*procIt)->is_node() )
                {
                    KTERROR( proclog, "Invalid processor entry: not a node" );
                    return false;
                }
                const scarab::param_node* procNode = &( (*procIt)->as_node() );

                if (! procNode->has("type"))
                {
                    KTERROR(proclog, "Unable to create processor: no processor type given");
                    return false;
                }
                string procType = procNode->get_value("type");

                string procName;
                if (! procNode->has("name"))
                {
                    KTINFO(proclog, "No name given for processor of type <" << procType << ">; using type as name.");
                    procName = procType;
                }
                else
                {
                    procName = procNode->get_value("name");
                }
                KTProcessor* newProc = fProcFactory->create(procType, procType);
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
        const scarab::param_array* connArray = node->array_at( "connections" );
        if (connArray == NULL)
        {
            KTWARN(proclog, "No connections were specified");
        }
        else
        {
            for( scarab::param_array::const_iterator connIt = connArray->begin(); connIt != connArray->end(); ++connIt )
            {
                if( ! (*connIt)->is_node() )
                {
                    KTERROR( proclog, "Invalid connection entry: not a node" );
                    return false;
                }
                const scarab::param_node* connNode = &( (*connIt)->as_node() );

                if ( ! connNode->has("signal") || ! connNode->has("slot") )
                {
                    KTERROR(proclog, "Signal/Slot connection information is incomplete!");
                    if (connNode->has("signal"))
                    {
                        KTWARN(proclog, "signal = " << connNode->get_value("signal"));
                    }
                    else
                    {
                        KTERROR(proclog, "signal = MISSING");
                    }

                    if (connNode->has("slot"))
                    {
                        KTWARN(proclog, "slot = " << connNode->get_value("slot"));
                    }
                    else
                    {
                        KTERROR(proclog, "slot = MISSING");
                    }
                    return false;
                }

                bool connReturn = false;
                if (connNode->has("order"))
                {
                    connReturn = MakeConnection(connNode->get_value("signal"), connNode->get_value("slot"), connNode->get_value< int >("order"));
                }
                else
                {
                    connReturn = MakeConnection(connNode->get_value("signal"), connNode->get_value("slot"));
                }
                if (! connReturn)
                {
                    KTERROR(proclog, "Unable to make connection <" << connNode->get_value("signal") << "> --> <" << connNode->get_value("slot") << ">");
                    return false;
                }

                KTINFO(proclog, "Signal <" << connNode->get_value("signal") << "> connected to slot <" << connNode->get_value("slot") << ">");
            }
        }


        // Finally, deal with processor-run specifications
        // The run queue is an array of processor names, or groups of names, which will be run sequentially.
        // If names are grouped (in another array), those in that group will be run in parallel.
        // In single threaded mode all threads will be run sequentially in the order they were specified.
        const scarab::param_array* rqArray = node->array_at( "run-queue" );
        if (rqArray == NULL)
        {
            KTWARN(proclog, "Run queue was not specified");
        }
        else
        {
            for (scarab::param_array::const_iterator rqIt = rqArray->begin(); rqIt != rqArray->end(); ++rqIt)
            {
                if ((*rqIt)->is_value())
                {
                    if (! PushBackToRunQueue((*rqIt)->as_value().as_string()))
                    {
                        KTERROR(proclog, "Unable to process run-queue entry: could not add processor to the queue");
                        return false;
                    }
                }
                else if ((*rqIt)->is_array())
                {
                    const scarab::param_array* rqNode = &( (*rqIt)->as_array() );
                    std::vector< std::string > names;

                    for (scarab::param_array::const_iterator rqArrayIt = rqNode->begin(); rqArrayIt != rqNode->end(); ++rqArrayIt)
                    {
                        if (! (*rqArrayIt)->is_value())
                        {
                            KTERROR(proclog, "Invalid run-queue array entry: not a value");
                            return false;
                        }
                        names.push_back((*rqArrayIt)->as_value().as_string());
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

    bool KTProcessorToolbox::ConfigureProcessors(const scarab::param_node* node)
    {
        for (ProcMapIt iter = fProcMap.begin(); iter != fProcMap.end(); iter++)
        {
            KTDEBUG(proclog, "Attempting to configure processor <" << iter->first << ">");
            string procName = iter->first;
            string nameUsed;
            const scarab::param_node* subNode = node->node_at(procName);
            if (subNode == NULL)
            {
                string configName = iter->second.fProc->GetConfigName();
                KTWARN(proclog, "Did not find a parameter node <" << procName << ">\n"
                        "\tWill check using the generic name of the processor, <" << configName << ">.");
                subNode = node->node_at(configName);
                if (subNode == NULL)
                {
                    KTWARN(proclog, "Did not find a parameter node <" << configName << ">\n"
                            "\tProcessor <" << iter->first << "> was not configured.");
                    continue;
                }
                nameUsed = configName;
            }
            else
            {
                nameUsed = procName;
            }
            if (! iter->second.fProc->Configure(subNode))
            {
                KTERROR(proclog, "An error occurred while configuring processor <" << iter->first << "> with parameter node <" << nameUsed << ">");
                return false;
            }
        }
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

    void KTProcessorToolbox::AsyncRun()
    {
        if( fDoRunThread != nullptr )
        {
            KTERROR( proclog, "It appears that a run has already been started" );
            return;
        }

        bool willRunSingleThreaded = fRunSingleThreaded;
#ifdef SINGLETHREADED
        willRunSingleThreaded = true;
#endif

        if( willRunSingleThreaded )
        {
            auto singleThreadRun = [&]()
            {
                try
                {
                    // reset the continue signaler
                    fContinueSignaler = std::promise< void >();
                    fMasterContSignal = fContinueSignaler.get_future();
                    if( ! fMasterContSignal.valid() )
                    {
                        KTERROR( proclog, "Invalid master continue-signal created" );
                        throw std::future_error( std::future_errc::no_state );
                    }
                    // copy for use in this function
                    std::shared_future< void > continueSignal = fMasterContSignal;

                    KTPROG( proclog, "Starting single-threaded processing" );

                    for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                    {
                        for (ThreadGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                        {
                            std::string procName( tgIter->fName );
                            KTINFO( proclog, "Starting processor <" << procName << ">" );
/*
                            KTDataPtrReturn dataRet;
                            fThreadFutures.push_back( dataRet.get_future() );
                            if( ! fThreadFutures.back().valid() )
                            {
                                KTERROR( proclog, "Invalid thread future created" );
                                throw std::future_error( std::future_errc::no_state );
                            }
*/
                            KTThreadReference thisThreadRef;

                            fThreadFutures.push_back( thisThreadRef.fDataPtrRet.get_future() );
                            if( ! fThreadFutures.back().valid() )
                            {
                                KTERROR( proclog, "Invalid thread future created" );
                                throw std::future_error( std::future_errc::no_state );
                            }

                            fThreadIndicators.push_back( KTThreadIndicator() );
                            fThreadIndicators.back().fBreakFlag = false;
                            fThreadIndicators.back().fContinueSignal = fMasterContSignal;

                            thisThreadRef.fThreadIndicator = &fThreadIndicators.back();
                            thisThreadRef.fProcTB = this;

                            //std::thread thread( &KTPrimaryProcessor::operator(), tgIter->fProc, std::move( dataRet ), fThreadPackets.back() );
                            tgIter->fProc->SetThreadRef( std::move( thisThreadRef ) );
                            std::thread thread( &KTPrimaryProcessor::operator(), tgIter->fProc );
                            tgIter->fProc->GetThreadRef()->fThread = &thread;

                            KTDEBUG( proclog, "Thread ID is <" << thread.get_id() << ">" );

                            bool stillRunning = true; // determines behavior that depends on whether a return from the thread was temporary or from the thread completing
                            do
                            {
                                std::future_status status;
                                do
                                {
                                    status = fThreadFutures.back().wait_for(std::chrono::milliseconds(500));
                                } while (status != std::future_status::ready);

                                stillRunning = false;
                                if( fThreadIndicators.back().fBreakFlag )
                                {
                                    KTDEBUG( proclog, "Breakpoint reached" );
                                    continueSignal.wait();
                                    stillRunning = true;
                                }
                                else
                                {
                                    try
                                    {
                                        fThreadFutures.back().get();
                                    }
                                    catch( std::exception& e )
                                    {
                                        // this transfers an exception thrown by a processor to the outer catch block in this function
                                        throw KTException() << "An error occurred while running processor <" << procName << ">: " << e.what();
                                    }
                                    stillRunning = false;
                                }
                            } while( stillRunning );

                            KTINFO( proclog, "Processor <" << procName << "> has finished" );

                            thread.join();
                        } // end for loop over the thread group
                    } // end for loop over the run-queue

                    KTPROG( proclog, "Processing is complete (single-threaded)" );
                }
                catch( ... )
                {
                    // exceptions thrown in this function or from within processors will end up here
                    fDoRunPromise.set_exception( std::current_exception() );
                }
                fDoRunPromise.set_value();
                return;
            };

            fDoRunThread = new std::thread( singleThreadRun );
        }
        else
        {
            auto multiThreadRun = [&]()
            {
                try
                {
                    std::shared_future< void > fMasterContSignal( fContinueSignaler.get_future() );
                    if( ! fMasterContSignal.valid() )
                    {
                        KTERROR( proclog, "Invalid master continue-signal created" );
                        throw std::future_error( std::future_errc::no_state );
                    }

                    KTPROG( proclog, "Starting multi-threaded processing" );

                    // TODO logic to start threads goes here

                    KTPROG( proclog, "Processing is complete (multi-threaded)" );
                }
                catch( ... )
                {
                    fDoRunPromise.set_exception( std::current_exception() );
                }
                fDoRunPromise.set_value();
                return;
            };

            fDoRunThread = new std::thread( multiThreadRun );
        }

        return;
    }

    bool KTProcessorToolbox::WaitForBreak()
    {
        std::future< void > doRunFuture = fDoRunPromise.get_future();

        std::future_status doRunStatus;
        do
        {
            doRunStatus = doRunFuture.wait_for( std::chrono::milliseconds( 500 ) );
        } while( doRunStatus != std::future_status::ready );

        if( fDoRunBreakFlag )
        {
            KTDEBUG( proclog, "Breakpoint reached" );
            return true;
        }

        try
        {
            doRunFuture.get();
            return false;
        }
        catch( std::exception& e )
        {
            // this transfers an exception thrown by a processor to the outer catch block in this function
            throw;
        }
    }

    void KTProcessorToolbox::Continue()
    {
        std::unique_lock< std::mutex > breakContLock( fBreakContMutex );

        // futures have been used; clear them to be replaced
        fThreadFutures.clear();

        // reset all break flags
        fDoRunBreakFlag = false;
        for( std::vector< KTThreadIndicator >::iterator tiIt = fThreadIndicators.begin(); tiIt != fThreadIndicators.end(); ++tiIt )
        {
            tiIt->fBreakFlag = false;
        }

        KTINFO( proclog, "Continuing from breakpoint" );
        // signal everything to resume
        fContinueSignaler.set_value();

        // reset the signaler and all signals
        // hopefully the delay of creating the new signaler and starting the for loop is enough that anything waiting on the old signal has already seen that signal and moved on
        fContinueSignaler = std::move( std::promise< void >() );
        for( std::vector< KTThreadIndicator >::iterator tiIt = fThreadIndicators.begin(); tiIt != fThreadIndicators.end(); ++tiIt )
        {
            tiIt->fContinueSignal = fContinueSignaler.get_future();
        }

        return;
    }

    void KTProcessorToolbox::InitiateBreak()
    {
        std::unique_lock< std::mutex > breakContLock( fBreakContMutex );

        // set all break flags
        fDoRunBreakFlag = true;
        for( std::vector< KTThreadIndicator >::iterator tiIt = fThreadIndicators.begin(); tiIt != fThreadIndicators.end(); ++tiIt )
        {
            tiIt->fBreakFlag = true;
        }

        return;
    }

    bool KTProcessorToolbox::Run()
    {
        AsyncRun();

        std::future< void > doRunFuture = fDoRunPromise.get_future();

        try
        {
            while( WaitForBreak() )
            {
                KTINFO( proclog, "Skipping breakpoint" );
                Continue();
            }
        }
        catch( std::exception& e )
        {
            KTERROR( proclog, "An error occurred: " << e.what() );
            return false;
        }

        return true;

        /*
        KTPROG(proclog, "Beginning processing . . .");
#ifndef SINGLETHREADED
        unsigned iGroup = 0;
#endif
        try
        {
            for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
            {
    //#ifdef SINGLETHREADED
                for (ThreadGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                {
                    std::promise< KTDataPtr > promise;
                    std::future< KTDataPtr > future = promise.get_future();
                    std::thread thread( &KTPrimaryProcessor::operator(), tgIter->fProc, std::move( promise ) );
                    if( ! future.valid() )
                    {
                        throw std::future_error( std::future_errc::no_state );
                    }
                    std::future_status status;
                    do
                    {
                        status = future.wait_for(std::chrono::milliseconds(500));
                    } while (status != std::future_status::ready);

                    try
                    {
                        future.get();
                    }
                    catch( std::exception& e )
                    {
                        throw KTException() << "An error occurred while running processor <" << tgIter->fName << ">: " << e.what();
                    }
                    thread.join();
                }
//#else
// for now, don't do this section of code
#if 0
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
#endif // this endif was here before i put in the temporary #if 0
            }
        }
        catch( std::exception& e )
        {
            KTERROR( proclog, e.what() );
            return false;
        }
        KTPROG(proclog, ". . . processing complete.");
        return true;
        */
    }


} /* namespace Nymph */
