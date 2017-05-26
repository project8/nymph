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
            fContinueCV(),
            fDoContinue( false ),
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
        catch (boost::exception& e)
        {
            KTERROR(proclog, "An error occurred while connecting signals and slots:\n"
                    << "\tSignal " << signalName << " from processor " << signalProcName << " (a.k.a. " << signalProc->GetConfigName() << ")" << '\n'
                    << "\tSlot " << slotName << " from processor " << slotProcName << " (a.k.a. " << slotProc->GetConfigName() << ")" << '\n'
                    << '\t' << diagnostic_information( e ));
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
        catch (boost::exception& e)
        {
            KTERROR(proclog, "Unable to set breakpoint: " << diagnostic_information( e ));
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
        catch (boost::exception& e)
        {
            KTERROR(proclog, "Unable to set breakpoint: " << diagnostic_information( e ));
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
                KTPROG( proclog, "Starting single-threaded processing" );

                for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                {
                    for (ThreadGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                    {
                        boost_unique_lock breakContLock( fBreakContMutex );

                        std::string procName( tgIter->fName );
                        KTINFO( proclog, "Starting processor <" << procName << ">" );

                        std::shared_ptr< KTThreadReference > thisThreadRef = std::make_shared< KTThreadReference >();
                        thisThreadRef->Name() = procName;

                        thisThreadRef->SetInitiateBreakFunc( [&](){ this->InitiateBreak(); } );
                        thisThreadRef->SetWaitForContinueFunc( [&]( boost_unique_lock& lock ){ this->WaitForContinue( lock ); } );

                        fThreadReferences.push_back( thisThreadRef );

                        boost::condition_variable threadStartedCV;
                        boost::mutex threadStartedMutex;
                        bool threadStartedFlag = false;

                        boost_unique_lock threadStartedLock( threadStartedMutex );
                        boost::thread thread( [&](){ tgIter->fProc->operator()( thisThreadRef, threadStartedCV, threadStartedFlag ); } );
                        KTDEBUG( proclog, "Thread ID is <" << thread.get_id() << ">; waiting for thread start" );
                        while( ! threadStartedFlag )
                        {
                            threadStartedCV.wait( threadStartedLock );
                        }
                        KTDEBUG( proclog, "Thread has started" );

                        KTDEBUG( proclog, "Thread ID is <" << thread.get_id() << ">" );

                        bool stillRunning = true; // determines behavior that depends on whether a return from the thread was temporary or from the thread completing
                        do
                        {
                            boost::future_status status;
                            do
                            {
                                status = fThreadReferences.back()->GetDataPtrRetFuture().wait_for( boost::chrono::milliseconds(500) );
                            } while (status != boost::future_status::ready);

                            stillRunning = false;
                            if( fThreadReferences.back()->GetBreakFlag() )
                            {
                                KTDEBUG( proclog, "Breakpoint reached (seen first in thread <" << procName << ">; may not be where breakpoint is set)" );

                                WaitForContinue( breakContLock );

                                KTDEBUG( proclog, "Breakpoint finished" );
                                stillRunning = true;
                            }
                            else
                            {
                                try
                                {
                                    fThreadReferences.back()->GetReturnValue();
                                    KTINFO( proclog, "Thread <" << procName << "> has finished" );
                                    fThreadReferences.pop_back();
                                }
                                catch( boost::exception& e )
                                {
                                    e << KTErrorMsgInfo< struct procTB_STRFinishing >( "There was an error while finishing thread <" + procName + "> and retrieving its results" );
                                    throw;
                                }
                                stillRunning = false;
                            }
                        } while( stillRunning );

                        KTINFO( proclog, "Processor <" << procName << "> has finished" );

                        thread.join();
                        fThreadReferences.clear();
                    } // end for loop over the thread group
                } // end for loop over the run-queue

                KTPROG( proclog, "Processing is complete (single-threaded)" );
            }
            catch( std::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                KTERROR( proclog, "Caught std::exception thrown in a processor or in the single-threaded run function: " << e.what() );
                KTWARN( proclog, "Setting boost::exception of do-run-promise in StartSingleThreadedRun" );
                fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            catch( boost::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                KTERROR( proclog, "Caught boost::exception thrown in a processor or in the single-threaded run function" );
                KTWARN( proclog, "Setting boost::exception of do-run-promise in StartSingleThreadedRun" );
                fDoRunPromise.set_exception( boost::current_exception() );
                return;
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
                KTPROG( proclog, "Starting multi-threaded processing" );

                for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                {
                    boost::thread_group threads;

                    { // scope for threadFuturesLock
                        boost_unique_lock breakContLock( fBreakContMutex );
                        //boost_unique_lock threadFuturesLock( fThreadReferencesMutex );

                        for (ThreadGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                        {
                            std::string procName( tgIter->fName );
                            KTINFO( proclog, "Starting processor <" << procName << ">" );

                            std::shared_ptr< KTThreadReference > thisThreadRef = std::make_shared< KTThreadReference >();
                            thisThreadRef->Name() = procName;

                            thisThreadRef->SetInitiateBreakFunc( [&](){ this->InitiateBreak(); } );
                            thisThreadRef->SetWaitForContinueFunc( [&]( boost_unique_lock& lock ){ this->WaitForContinue( lock ); } );
                            fThreadReferences.push_back( thisThreadRef );

                            boost::condition_variable threadStartedCV;
                            boost::mutex threadStartedMutex;
                            bool threadStartedFlag = false;

                            boost_unique_lock threadStartedLock( threadStartedMutex );
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
                        auto finishedFuturePtr = boost::wait_for_any( KTThreadRefFutureIterator(fThreadReferences.begin()), KTThreadRefFutureIterator(fThreadReferences.end()) );
                        boost_unique_lock breakContLock( fBreakContMutex );

                        size_t iThread = finishedFuturePtr - KTThreadRefFutureIterator(fThreadReferences.begin());
                        auto finishedReferencePtr = fThreadReferences.begin() + iThread;
                        std::string threadName( (*finishedReferencePtr)->Name() );
                        KTDEBUG( proclog, "Thread <" << iThread << " (" << threadName << ") > has stopped for some reason" );

                        stillRunning = false;
                        if( fDoRunBreakFlag )
                        {
                            // a breakpoint has been reached
                            KTDEBUG( proclog, "Breakpoint reached (seen first in thread <" << threadName << ">; may not be where breakpoint is set)" );

                            // wait here for the user to continue
                            WaitForContinue( breakContLock );

                            KTDEBUG( proclog, "Breakpoint finished" );
                            stillRunning = true;
                            //continueSignal = fMasterContSignal; // refresh the local copy of the shared future
                        }
                        else
                        {
                            // we're finished a thread; get its results
                            try
                            {
                                KTINFO( proclog, "Thread <" << threadName << "> has finished" );
                                finishedFuturePtr->get();
                                fThreadReferences.erase( finishedReferencePtr );
                            }
                            catch( boost::exception& e )
                            {
                                e << KTErrorMsgInfo< struct procTB_MTRFinishing >( "There was an error while finishing thread <" + threadName + "> and retrieving its results" );
                                throw;
                            }

                            if( fThreadReferences.empty() ) stillRunning = false;
                            else stillRunning = true;
                        }
                    } while( stillRunning );

                    KTDEBUG( proclog, "Joining threads" );
                    threads.join_all();
                    boost_unique_lock breakContLock( fBreakContMutex );
                    fThreadReferences.clear();
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
                KTERROR( proclog, "Caught boost::exception thrown in a processor or in the multi-threaded run function" );
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
        boost_unique_lock breakContLock( fBreakContMutex );

        if( fDoRunBreakFlag )
        {
            KTWARN( proclog, "Can't wait for a break; Already at a breakpoint" );
            return true;
        }

        boost::shared_future< void > doRunFuture = fDoRunFuture;
        if( ! doRunFuture.valid() )
        {
            BOOST_THROW_EXCEPTION( KTException() << "Cannot wait for a break in the current state (the \"DoRun\" future does not have a valid shared state)" << eom );
        }

        breakContLock.unlock();

        // block here until there's a break
        doRunFuture.wait();

        if( fDoRunBreakFlag )
        {
            KTDEBUG( proclog, "Breakpoint reached (the wait is over)" );
            return true;
        }

        try
        {
            doRunFuture.get();
            KTDEBUG( proclog, "End-of-run reached (the wait-for-break is over)" );
            return false;
        }
        catch( boost::exception& e )
        {
            KTERROR( proclog, "An error occurred during processing: " << boost::diagnostic_information( e ) );
            return false;
        }
    }

    void KTProcessorToolbox::WaitForEndOfRun()
    {
        // WaitForBreak() and WaitForContinue() throw boost::exception for problems with the future or promise objects

        KTDEBUG( proclog, "Waiting for end-of-run" );
        while( WaitForBreak() )
        {
            KTDEBUG( proclog, "Reached breakpoint; waiting for continue" );
            boost::mutex localMutex;
            boost_unique_lock localLock( localMutex );
            WaitForContinue( localLock );
            KTDEBUG( proclog, "Processing resuming; waiting for end-of-run" );
        }
        KTDEBUG( proclog, "End-of-run reached" );

        return;
    }

    void KTProcessorToolbox::Continue()
    {
        if( ! fDoRunBreakFlag )
        {
            KTWARN( proclog, "Not currently at a breakpoint" );
            return;
        }

        boost_unique_lock breakContLock( fBreakContMutex );

        // reset all break flags
        fDoRunBreakFlag = false;
        std::vector< boost_unique_lock > trLocks;
        for( auto trIt = fThreadReferences.begin(); trIt != fThreadReferences.end(); ++trIt )
        {
            trLocks.emplace_back( (*trIt)->Mutex() );
            (*trIt)->SetBreakFlag( false );
            (*trIt)->RefreshDataPtrRet();
        }

        // reset the do-run promise and future
        fDoRunPromise = boost::promise< void >();
        fDoRunFuture = fDoRunPromise.get_future().share();

        KTINFO( proclog, "Continuing from breakpoint" );
        // signal everything to resume
        fDoContinue = true;
        fContinueCV.notify_all();

        return;
    }

    KTDataPtr KTProcessorToolbox::GetData( const std::string& threadName )
    {
        //boost_unique_lock threadFuturesLock( fThreadReferencesMutex );
        boost_unique_lock breakContLock( fBreakContMutex );

        auto trIt = fThreadReferences.begin();
        for( ; trIt != fThreadReferences.end(); ++trIt )
        {
            if( (*trIt)->Name() == threadName ) break;
        }
        if( trIt == fThreadReferences.end() )
        {
            KTWARN( proclog, "Did not find thread <" << threadName << ">" );
            BOOST_THROW_EXCEPTION( KTException() << "Did not find thread <" << threadName << ">" << eom );
        }

        boost_unique_lock lock( (*trIt)->Mutex() );
        return (*trIt)->GetReturnValue();
    }

    void KTProcessorToolbox::InitiateBreak()
    {
        boost_unique_lock breakContLock( fBreakContMutex );
        if( fDoRunBreakFlag ) return;

        // set the continue flag to false so that things will wait for the continue
        fDoContinue = false;

        // set all break flags
        // master break flag
        fDoRunBreakFlag = true;
        // thread break flags
        for( auto trIt = fThreadReferences.begin(); trIt != fThreadReferences.end(); ++trIt )
        {
            boost_unique_lock lock( (*trIt)->Mutex() );
            (*trIt)->SetBreakFlag( true );
        }

        // use the do-run promise to signal the break
        KTWARN( proclog, "Setting value of do-run-promise" );
        fDoRunPromise.set_value();

        return;
    }

    void KTProcessorToolbox::CancelThreads()
    {
        boost_unique_lock breakContLock( fBreakContMutex );
        for( auto trIt = fThreadReferences.begin(); trIt != fThreadReferences.end(); ++trIt )
        {
            boost_unique_lock lock( (*trIt)->Mutex() );
            (*trIt)->SetCanceled( true );
        }
        return;
    }

    bool KTProcessorToolbox::Run()
    {
        // can throw boost::exception

        AsyncRun();

        WaitForEndOfRun();

        JoinRunThread();

        return true;
    }

} /* namespace Nymph */
