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
#include "param_codec.hh"

#include <boost/exception/get_error_info.hpp>
#include <boost/thread.hpp>

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
            fRunSingleThreaded( false ),
            fRunQueue(),
            fProcMap(),
            fThreadReferences(),
            fThreadFuturesMutex(),
            //fThreadFutures(),
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
        CancelThreads();

        JoinRunThread();

        ClearProcessors();
    }

    bool KTProcessorToolbox::Configure(const scarab::param_node* node)
    {
        KTPROG(proclog, "Configuring . . .");

        SetRunSingleThreaded( node->get_value( "single-threaded", fRunSingleThreaded ) );

        // Deal with "processor" blocks
        if (! node->has("processors"))
        {
            KTWARN(proclog, "No processors were specified");
        }
        else
        {
            const scarab::param_array& procArray = node->array_at( "processors" );
            for( scarab::param_array::const_iterator procIt = procArray.begin(); procIt != procArray.end(); ++procIt )
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
        if (! node->has("connections"))
        {
            KTWARN(proclog, "No connections were specified");
        }
        else
        {
            const scarab::param_array& connArray = node->array_at( "connections" );
            for( scarab::param_array::const_iterator connIt = connArray.begin(); connIt != connArray.end(); ++connIt )
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

                if (connNode->has("breakpoint"))
                {
                    if (! SetBreakpoint(connNode->get_value("slot")))
                    {
                        KTERROR(proclog, "Unable to set breakpoint on <" << connNode->get_value("slot"));
                        return false;
                    }
                }

                KTINFO(proclog, "Signal <" << connNode->get_value("signal") << "> connected to slot <" << connNode->get_value("slot") << ">");
            }
        }


        // Finally, deal with processor-run specifications
        // The run queue is an array of processor names, or groups of names, which will be run sequentially.
        // If names are grouped (in another array), those in that group will be run in parallel.
        // In single threaded mode all threads will be run sequentially in the order they were specified.
        if (! node->has("run-queue"))
        {
            KTWARN(proclog, "Run queue was not specified");
        }
        else
        {
            const scarab::param_array& rqArray = node->array_at( "run-queue" );
            for (scarab::param_array::const_iterator rqIt = rqArray.begin(); rqIt != rqArray.end(); ++rqIt)
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
            string nameUsed(procName);
            if (! node->has(nameUsed))
            {
                nameUsed = iter->second.fProc->GetConfigName();
                if (! node->has(nameUsed))
                {
                    KTWARN(proclog, "Did not find a parameter node <" << procName << "> or <" << nameUsed << ">\n"
                            "\tProcessor <" << procName << "> was not configured.");
                    continue;
                }
            }
            const scarab::param_node& subNode = node->node_at(nameUsed);
            if (! iter->second.fProc->Configure(&subNode))
            {
                KTERROR(proclog, "An error occurred while configuring processor <" << procName << "> with parameter node <" << nameUsed << ">");
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
        return ConfigureProcessors( &translator.read_string( config, &optNode )->as_node() );
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
        ProcMapIt it = fProcMap.find(procName);
        if (it == fProcMap.end())
        {
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

    bool KTProcessorToolbox::SetBreakpoint(const std::string& slot)
    {
        string slotProcName, slotName;
        if (! ParseSignalSlotName(slot, slotProcName, slotName))
        {
            KTERROR(proclog, "Unable to parse slot name: <" << slot << ">");
            return false;
        }

        return SetBreakpoint(slotProcName, slotName);
    }

    bool KTProcessorToolbox::SetBreakpoint(const std::string& slotProcName, const std::string& slotName)
    {
        KTProcessor* slotProc = GetProcessor(slotProcName);
        if (slotProc == NULL)
        {
            KTERROR(proclog, "Processor named <" << slotProcName << "> was not found!");
            return false;
        }

        try
        {
            slotProc->SetDoBreakpoint(slotName, true);
            return true;
        }
        catch (std::exception& e)
        {
            KTERROR(proclog, "Unable to set breakpoint: " << e.what());
            return false;
        }
    }

    bool KTProcessorToolbox::RemoveBreakpoint(const std::string& slot)
    {
        string slotProcName, slotName;
        if (! ParseSignalSlotName(slot, slotProcName, slotName))
        {
            KTERROR(proclog, "Unable to parse slot name: <" << slot << ">");
            return false;
        }

        return RemoveBreakpoint(slotProcName, slotName);
    }

    bool KTProcessorToolbox::RemoveBreakpoint(const std::string& slotProcName, const std::string& slotName)
    {
        KTProcessor* slotProc = GetProcessor(slotProcName);
        if (slotProc == NULL)
        {
            KTERROR(proclog, "Processor named <" << slotProcName << "> was not found!");
            return false;
        }

        try
        {
            slotProc->SetDoBreakpoint(slotName, false);
            return true;
        }
        catch (std::exception& e)
        {
            KTERROR(proclog, "Unable to set breakpoint: " << e.what());
            return false;
        }
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

        fDoRunPromise = boost::promise< void >();
        fDoRunFuture = fDoRunPromise.get_future().share();

        bool willRunSingleThreaded = fRunSingleThreaded;
#ifdef SINGLETHREADED
        willRunSingleThreaded = true;
#endif

        if( willRunSingleThreaded )
        {
            StartSingleThreadedRun();
        }
        else
        {
            StartMultiThreadedRun();
        }

        return;
    }

    void KTProcessorToolbox::StartSingleThreadedRun()
    {
        auto singleThreadRun = [&]()
        {
            try
            {
                // reset the continue signaler
                fContinueSignaler = boost::promise< void >();
                fMasterContSignal = fContinueSignaler.get_future().share();
                if( ! fMasterContSignal.valid() )
                {
                    KTERROR( proclog, "Invalid master continue-signal created" );
                    throw KTException() << "Invalid master continue-signal created";
                }
                // copy for use in this function
                boost::shared_future< void > continueSignal = fMasterContSignal;

                KTPROG( proclog, "Starting single-threaded processing" );

                for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                {
                    for (ThreadGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                    {
                        boost::unique_lock< boost::mutex > threadFuturesLock( fThreadFuturesMutex );

                        std::string procName( tgIter->fName );
                        KTINFO( proclog, "Starting processor <" << procName << ">" );

                        //KTThreadReference thisThreadRef;
                        std::shared_ptr< KTThreadReference > thisThreadRef = std::make_shared< KTThreadReference >();
                        thisThreadRef->fName = procName;

                        //fThreadFutures.push_back( std::move( thisThreadRef.fDataPtrRet.get_future() ) );
                        //fThreadNames.push_back( procName );
                        //auto& futureNameIndex = fThreadFutures.get<1>();

                        //if( ! fThreadFutures.back().valid() )
                        //{
                        //    KTERROR( proclog, "Invalid thread future created" );
                        //    throw KTException() << "Invalid thread future created";
                        //}

                        //fThreadIndicators.emplace_back( new KTThreadIndicator() );
                        //fThreadIndicators.back()->fContinueSignal = fMasterContSignal;
                        thisThreadRef->fContinueSignal = fMasterContSignal;

                        //thisThreadRef->fThreadIndicator = fThreadIndicators.back();
                        thisThreadRef->fInitiateBreakFunc = [&](){ this->InitiateBreak(); };
                        //thisThreadRef->fRefreshFutureFunc = [&]( const std::string& name, Future&& ret ){ this->TakeFuture( name, std::move(ret) ); };

                        fThreadReferences.push_back( thisThreadRef );

                        boost::condition_variable threadStartedCV;
                        boost::mutex threadStartedMutex;
                        bool threadStartedFlag = false;

                        boost::unique_lock< boost::mutex > threadStartedLock( threadStartedMutex );
                        boost::thread thread( [&](){ tgIter->fProc->operator()( thisThreadRef, threadStartedCV, threadStartedFlag ); } );
                        KTDEBUG( proclog, "Thread ID is <" << thread.get_id() << ">; waiting for thread start" );
                        while( ! threadStartedFlag )
                        {
                            threadStartedCV.wait( threadStartedLock );
                        }
                        KTDEBUG( proclog, "Thread has started" );

                        KTDEBUG( proclog, "Thread ID is <" << thread.get_id() << ">" );

                        threadFuturesLock.unlock();

                        bool stillRunning = true; // determines behavior that depends on whether a return from the thread was temporary or from the thread completing
                        do
                        {
                            boost::future_status status;
                            do
                            {
                                status = fThreadReferences.back()->fDataPtrRetFuture.wait_for( boost::chrono::milliseconds(500) );
                            } while (status != boost::future_status::ready);

                            stillRunning = false;
                            if( fThreadReferences.back()->fBreakFlag )
                            {
                                KTDEBUG( proclog, "Breakpoint reached (seen first in thread <" << fThreadReferences.back()->fName << ">; may not be where breakpoint is set)" );
                                continueSignal.wait();
                                KTDEBUG( proclog, "Breakpoint finished" );
                                stillRunning = true;
                                continueSignal = fMasterContSignal; // refresh the local copy of the shared future
                            }
                            else
                            {
                                try
                                {
                                    fThreadReferences.back()->fDataPtrRetFuture.get();
                                    KTINFO( proclog, "Thread <" << fThreadReferences.back()->fName << "> has finished" );
                                    fThreadReferences.pop_back();
                                }
                                catch( boost::exception& e )
                                {
                                    KTERROR( proclog, "Thread <" << fThreadReferences.back()->fName << "> threw an error: " << diagnostic_information( e ) );
                                    // this transfers an exception thrown by a processor to the outer catch block in this function
                                    throw KTException() << "An error occurred while running processor <" << procName << ">: " << diagnostic_information( e );
                                }
                                stillRunning = false;
                            }
                        } while( stillRunning );

                        KTINFO( proclog, "Processor <" << procName << "> has finished" );

                        thread.join();
                        fThreadReferences.clear();
                        //fThreadFutures.clear();
                        //fThreadNames.clear();
                    } // end for loop over the thread group
                } // end for loop over the run-queue

                KTPROG( proclog, "Processing is complete (single-threaded)" );
            }
            catch( std::exception& e )
            {
                KTDEBUG( proclog, "Caught exception thrown in a processor or in the single-threaded run function: " << e.what() );
                // exceptions thrown in this function or from within processors will end up here
                fDoRunPromise.set_exception( std::current_exception() );
            }
            catch( boost::exception& e )
            {
                KTDEBUG( proclog, "Caught exception thrown in a processor or in the single-threaded run function: " << diagnostic_information( e ) );
                // exceptions thrown in this function or from within processors will end up here
                fDoRunPromise.set_exception( std::current_exception() );
            }
            fDoRunPromise.set_value();
            return;
        }; // end single-threaded run lambda

        fDoRunThread = new boost::thread( singleThreadRun );
        return;
    }
    void KTProcessorToolbox::StartMultiThreadedRun()
    {
        auto multiThreadRun = [&]()
        {
            try
            {
                // reset the continue signaler
                fContinueSignaler = boost::promise< void >();
                fMasterContSignal = fContinueSignaler.get_future().share();
                if( ! fMasterContSignal.valid() )
                {
                    KTERROR( proclog, "Invalid master continue-signal created" );
                    throw KTException() << "Invalid master continue-signal created";
                }
                // copy for use in this function
                boost::shared_future< void > continueSignal = fMasterContSignal;

                KTPROG( proclog, "Starting multi-threaded processing" );

                for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                {
                    boost::thread_group threads;

                    { // scope for threadFuturesLock
                        boost::unique_lock< boost::mutex > threadFuturesLock( fThreadFuturesMutex );

                        for (ThreadGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                        {
                            std::string procName( tgIter->fName );
                            KTINFO( proclog, "Starting processor <" << procName << ">" );

                            //KTThreadReference thisThreadRef;
                            std::shared_ptr< KTThreadReference > thisThreadRef = std::make_shared< KTThreadReference >();
                            thisThreadRef->fName = procName;

                            //fThreadFutures.push_back( std::move( thisThreadRef.fDataPtrRet.get_future() ) );
                            //fThreadNames.push_back( procName );

                            //fThreadIndicators.emplace_back( new KTThreadIndicator() );
                            //fThreadIndicators.back()->fContinueSignal = fMasterContSignal;
                            thisThreadRef->fContinueSignal = fMasterContSignal;

                            //thisThreadRef->fThreadIndicator = fThreadIndicators.back();
                            thisThreadRef->fInitiateBreakFunc = [&](){ this->InitiateBreak(); };
                            //thisThreadRef->fRefreshFutureFunc = [&]( const std::string& name, Future&& ret ){ this->TakeFuture( name, std::move(ret) ); };
                            boost::condition_variable threadStartedCV;
                            boost::mutex threadStartedMutex;
                            bool threadStartedFlag = false;

                            fThreadReferences.push_back( thisThreadRef );

                            boost::unique_lock< boost::mutex > threadStartedLock( threadStartedMutex );
                            boost::thread* thisThread = new boost::thread( [&](){ tgIter->fProc->operator()( thisThreadRef, threadStartedCV, threadStartedFlag ); } );
                            KTDEBUG( proclog, "Thread ID is <" << thisThread->get_id() << ">; waiting for thread start" );
                            while( ! threadStartedFlag )
                            {
                                threadStartedCV.wait( threadStartedLock );
                            }
                            KTDEBUG( proclog, "Thread has started" );

                            threads.add_thread( thisThread );

                        }// end for loop over the thread group
                    } // end scope for threadFuturesLock

                    bool stillRunning = true; // determines behavior that depends on whether a return from the thread was temporary or from the thread completing
                    do
                    {
                        boost::unique_lock< boost::mutex > threadFuturesLock( fThreadFuturesMutex );
                        //KTThreadRefFutureIterator finishedFuturePtr( fThreadReferences.begin());
                        //auto finishedFuturePtr = boost::wait_for_any( futureIndex.begin(), futureIndex.end() );
                        //auto finishedFuturePtr = boost::wait_for_any( fThreadFutures.begin(), fThreadFutures.end() );
                        auto finishedFuturePtr = boost::wait_for_any( KTThreadRefFutureIterator(fThreadReferences.begin()), KTThreadRefFutureIterator(fThreadReferences.end()) );
                        size_t iThread = finishedFuturePtr - KTThreadRefFutureIterator(fThreadReferences.begin());
                        auto finishedReferencePtr = fThreadReferences.begin() + iThread;
                        std::string threadName( (*finishedReferencePtr)->fName );
                        //KTDEBUG( proclog, "Thread <" << iThread << " (" << threadName << ") > has stopped for some reason" );
                        KTWARN( proclog, "Thread <" << iThread << " (" << threadName << ") > has stopped for some reason" ); // TODO: swtich this back to DEBUG

                        stillRunning = false;
                        if( fDoRunBreakFlag )
                        {
                            // a breakpoint has been reached
                            //KTDEBUG( proclog, "Breakpoint reached (seen first in thread <" << threadName << ">; may not be where breakpoint is set)" );
                            KTWARN( proclog, "Breakpoint reached (seen first in thread <" << threadName << ">; may not be where breakpoint is set)" ); //TODO: switch this back to debug
                            threadFuturesLock.unlock();
                            // wait here for the user to continue
                            continueSignal.wait();
                            KTDEBUG( proclog, "Breakpoint finished" );
                            boost::unique_lock< boost::mutex > breakContLock( fBreakContMutex );
                            stillRunning = true;
                            continueSignal = fMasterContSignal; // refresh the local copy of the shared future
                        }
                        else
                        {
                            // we're finished a thread; get its results
                            try
                            {
                                //futureIndex.modify( finishedFuturePtr, [](LabeledFuture& lFuture){ lFuture.fFuture.get(); } );
                                //KTINFO( proclog, "Thread <" << threadName << "> has finished" );
                                KTWARN( proclog, "Thread <" << threadName << "> has finished" ); // TODO: switch this back to INFO
                                finishedFuturePtr->get();
                                fThreadReferences.erase( finishedReferencePtr );
                                //fThreadFutures.erase( finishedFuturePtr );
                                //fThreadNames.erase( fThreadNames.begin() + iThread );
                            }
                            catch( boost::exception& e )
                            {
                                KTERROR( proclog, "Thread <" << threadName << "> threw an error: " << diagnostic_information( e ) );
                                // this transfers an exception thrown by a processor to the outer catch block in this function
                                throw KTException() << "An error occurred while running processor <" << threadName << ">: " << diagnostic_information( e );
                            }

                            if( fThreadReferences.empty() ) stillRunning = false;
                            else stillRunning = true;
                        }
                    } while( stillRunning );

                    KTDEBUG( proclog, "Joining threads" );
                    threads.join_all();
                    //fThreadIndicators.clear();
                    boost::unique_lock< boost::mutex > threadFuturesLock( fThreadFuturesMutex );
                    fThreadReferences.clear();
                    //fThreadFutures.clear();
                    //fThreadNames.clear();
                } // end for loop over the run-queue

                KTPROG( proclog, "Processing is complete (multi-threaded)" );
            }
            catch( std::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                KTERROR( proclog, "Caught std::exception thrown in a processor or in the multi-threaded run function: " << e.what() );
                KTWARN( proclog, "Setting boost::exception of do-run-promise in StartMultiThreadedRun" );
                fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            catch( boost::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                KTERROR( proclog, "Caught std::exception thrown in a processor or in the multi-threaded run function: " << diagnostic_information( e ) );
                KTWARN( proclog, "Setting boost::exception of do-run-promise in StartMultiThreadedRun" );
                fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            KTWARN( proclog, "Setting value of do-run-promise in StartMultiThreadedRun" );
            fDoRunPromise.set_value();
            return;
        }; // end multi-threaded run lambda

        fDoRunThread = new boost::thread( multiThreadRun );
        return;
    }


    bool KTProcessorToolbox::WaitForBreak()
    {
        boost::unique_lock< boost::mutex > breakContLock( fBreakContMutex );

        if( fDoRunBreakFlag )
        {
            KTWARN( proclog, "Can't wait for a break; Already at a breakpoint" );
            return true;
        }

        boost::shared_future< void > doRunFuture = fDoRunFuture;
        if( ! doRunFuture.valid() )
        {
            throw KTException() << "Cannot wait for a break in the current state (the \"DoRun\" future does not have a valid shared state)";
        }

        breakContLock.unlock();

        doRunFuture.wait();

        if( fDoRunBreakFlag )
        {
            KTDEBUG( proclog, "Breakpoint reached (the wait is over)" );
            return true;
        }

        try
        {
            doRunFuture.get(); // TODO: do we really need this?  NSO, 5/23/17
            KTDEBUG( proclog, "End-of-run reached (the wait-for-break is over)" );
            return false;
        }
        catch( std::exception& e )
        {
            KTERROR( proclog, "An error occurred during processing: " << e.what() );
            return false;
        }
    }

    void KTProcessorToolbox::WaitForEndOfRun()
    {
        try
        {
            KTDEBUG( proclog, "Waiting for end-of-run" );
            while( WaitForBreak() )
            {
                KTDEBUG( proclog, "Reached breakpoint; waiting for continue" );
                WaitForContinue();
                KTDEBUG( proclog, "Processing resuming; waiting for end-of-run" );
            }
            KTDEBUG( proclog, "End-of-run reached" );
        }
        catch( std::exception& e )
        {
            throw;
        }
        return;
    }

    void KTProcessorToolbox::Continue()
    {
        if( ! fDoRunBreakFlag )
        {
            KTWARN( proclog, "Not currently at a breakpoint" );
            return;
        }

        boost::unique_lock< boost::mutex > breakContLock( fBreakContMutex );
        boost::unique_lock< boost::mutex > threadFuturesLock( fThreadFuturesMutex );

        // futures have been used; clear them to be replaced
        //fThreadFutures.clear();

        // reset all break flags
        fDoRunBreakFlag = false;
        for( auto tiIt = fThreadReferences.begin(); tiIt != fThreadReferences.end(); ++tiIt )
        {
            (*tiIt)->fBreakFlag = false;
            (*tiIt)->RefreshDataPtrRet();
        }

        // reset the do-run promise and future
        fDoRunPromise = boost::promise< void >();
        fDoRunFuture = fDoRunPromise.get_future().share();

        KTINFO( proclog, "Continuing from breakpoint" );
        // signal everything to resume
        KTWARN( proclog, "Setting value of continue-signaler promise" );
        fContinueSignaler.set_value();

        // reset the signaler and all signals
        // hopefully the delay of creating the new signaler and starting the for loop is enough that anything waiting on the old signal has already seen that signal and moved on
        fContinueSignaler = boost::promise< void >();
        fMasterContSignal = fContinueSignaler.get_future().share();
        for( auto tiIt = fThreadReferences.begin(); tiIt != fThreadReferences.end(); ++tiIt )
        {
            (*tiIt)->fContinueSignal = fMasterContSignal;
        }

        return;
    }

    KTDataPtr KTProcessorToolbox::GetData( const std::string& threadName )
    {
        boost::unique_lock< boost::mutex > threadFuturesLock( fThreadFuturesMutex );

        auto tnIt = fThreadReferences.begin();
        for( ; tnIt != fThreadReferences.end(); ++tnIt )
        {
            if( (*tnIt)->fName == threadName ) break;
        }
        if( tnIt == fThreadReferences.end() )
        {
            KTWARN( proclog, "Did not find thread <" << threadName << ">" );
            throw KTException() << "Did not find thread <" << threadName << ">" ;
        }

        return (*tnIt)->fDataPtrRetFuture.get();
    }

    void KTProcessorToolbox::InitiateBreak()
    {
        boost::unique_lock< boost::mutex > breakContLock( fBreakContMutex );
        if( fDoRunBreakFlag ) return;

        // set all break flags
        // master break flag
        fDoRunBreakFlag = true;
        // thread break flags
        for( auto tiIt = fThreadReferences.begin(); tiIt != fThreadReferences.end(); ++tiIt )
        {
            (*tiIt)->fBreakFlag = true;
        }

        // use the do-run promise to signal the break
        KTWARN( proclog, "Setting value of do-run-promise" );
        fDoRunPromise.set_value();

        return;
    }

    void KTProcessorToolbox::CancelThreads()
    {
        boost::unique_lock< boost::mutex > breakContLock( fBreakContMutex );
        for( auto tiIt = fThreadReferences.begin(); tiIt != fThreadReferences.end(); ++tiIt )
        {
            (*tiIt)->fCanceled = true;
        }
        return;
    }

    bool KTProcessorToolbox::Run()
    {
        try
        {
            AsyncRun();

            WaitForEndOfRun();

            JoinRunThread();
        }
        catch( std::exception& e )
        {
            throw;
        }
        return true;
    }

} /* namespace Nymph */
