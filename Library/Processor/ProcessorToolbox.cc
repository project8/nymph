/*
 * ProcessorToolbox.cc
 *
 *  Created on: Sep 27, 2012
 *      Author: N.S. Oblath
 */

#include "ProcessorToolbox.hh"

#include "Exception.hh"
#include "PrimaryProcessor.hh"

#include "factory.hh"
#include "logger.hh"
#include "param_codec.hh"

#include <map>
#include <thread>

//#include <boost/exception/get_error_info.hpp>
//#include <boost/thread.hpp>

using std::deque;
using std::set;
using std::string;
using std::vector;

namespace Nymph
{
    LOGGER(proclog, "ProcessorToolbox");

    ProcessorToolbox::ProcessorToolbox( const std::string& name ) :
            fProcFactory( scarab::factory< Processor, const std::string& >::get_instance() ),
            fRunSingleThreaded( false ),
            fRunQueue(),
            fProcMap(),
            fControl( new SharedControl() )/*,
            fThreadReferences(),
            fContinueCV(),
            fDoContinue( false ),
            fBreakContMutex(),
            fDoRunThread( nullptr ),
            fDoRunPromise(),
            fDoRunBreakFlag( false )*/
    {
    }

    ProcessorToolbox::~ProcessorToolbox()
    {
        //CancelThreads();

        //JoinRunThread();

        ClearProcessors();
    }

    bool ProcessorToolbox::Configure( const scarab::param_node& node )
    {
        LPROG(proclog, "Configuring . . .");

        SetRunSingleThreaded( node.get_value( "single-threaded", fRunSingleThreaded ) );

        // Deal with "processor" blocks
        if( ! node.has("processors") )
        {
            LWARN(proclog, "No processors were specified");
        }
        else
        {
            const scarab::param_array& procArray = node["processors"].as_array();
            for( scarab::param_array::const_iterator procIt = procArray.begin(); procIt != procArray.end(); ++procIt )
            {
                if( ! procIt->is_node() )
                {
                    LERROR( proclog, "Invalid processor entry (not a node): " << *procIt );
                    return false;
                }
                const scarab::param_node& procNode = procIt->as_node();

                if (! procNode.has("type"))
                {
                    LERROR(proclog, "Unable to create processor: no processor type given");
                    return false;
                }
                string procType = procNode["type"]().as_string();

                string procName;
                if (! procNode.has("name"))
                {
                    LINFO(proclog, "No name given for processor of type <" << procType << ">; using type as name.");
                    procName = procType;
                }
                else
                {
                    procName = procNode["name"]().as_string();
                }
                std::shared_ptr< Processor > newProc ( fProcFactory->create(procType, procType));
                if (newProc == NULL)
                {
                    LERROR(proclog, "Unable to create processor of type <" << procType << ">");
                    return false;
                }

                if (! AddProcessor(procName, newProc))
                {
                    LERROR(proclog, "Unable to add processor <" << procName << ">");
                    //delete newProc; //not required for smart pointers
                    return false;
                }
            }
        }


        // Then deal with connections"
        if (! node.has("connections"))
        {
            LWARN(proclog, "No connections were specified");
        }
        else
        {
            const scarab::param_array& connArray = node["connections"].as_array();
            for( scarab::param_array::const_iterator connIt = connArray.begin(); connIt != connArray.end(); ++connIt )
            {
                if( ! connIt->is_node() )
                {
                    LERROR( proclog, "Invalid connection entry: not a node" );
                    return false;
                }
                const scarab::param_node& connNode = connIt->as_node();

                if ( ! connNode.has("signal") || ! connNode.has("slot") )
                {
                    LERROR(proclog, "Signal/Slot connection information is incomplete!");
                    if (connNode.has("signal"))
                    {
                        LWARN(proclog, "signal = " << connNode["signal"]());
                    }
                    else
                    {
                        LERROR(proclog, "signal = MISSING");
                    }

                    if (connNode.has("slot"))
                    {
                        LWARN(proclog, "slot = " << connNode["slot"]());
                    }
                    else
                    {
                        LERROR(proclog, "slot = MISSING");
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
                    LERROR(proclog, "Unable to make connection <" << connNode["signal"]() << "> --> <" << connNode["slot"]() << ">");
                    return false;
                }

                if (connNode.has("breakpoint"))
                {
                    if (! SetBreakpoint(connNode["slot"]().as_string()))
                    {
                        LERROR(proclog, "Unable to set breakpoint on <" << connNode["slot"]());
                        return false;
                    }
                }

                LINFO(proclog, "Signal <" << connNode["signal"]() << "> connected to slot <" << connNode["slot"]() << ">");
            }
        }


        // Finally, deal with processor-run specifications
        // The run queue is an array of processor names, or groups of names, which will be run sequentially.
        // If names are grouped (in another array), those in that group will be run in parallel.
        // In single threaded mode all threads will be run sequentially in the order they were specified.
        if (! node.has("run-queue"))
        {
            LWARN(proclog, "Run queue was not specified");
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
                        LERROR(proclog, "Unable to process run-queue entry: could not add processor to the queue");
                        return false;
                    }
                }
                else if (rqIt->is_array())
                {
                    const scarab::param_array* rqNode = &( rqIt->as_array() );
                    std::vector< std::string > names;

                    for (scarab::param_array::const_iterator rqArrayIt = rqNode->begin(); rqArrayIt != rqNode->end(); ++rqArrayIt)
                    {
                        if (! rqArrayIt->is_value())
                        {
                            LERROR(proclog, "Invalid run-queue array entry: not a value");
                            return false;
                        }
                        names.push_back((*rqArrayIt)().as_string());
                    }

                    if (! PushBackToRunQueue(names))
                    {
                        LERROR(proclog, "Unable to process run-queue entry: could not add list of processors to the queue");
                        return false;
                    }
                }
                else
                {
                    LERROR(proclog, "Invalid run-queue entry: not a value or array");
                    return false;
                }
            }
        }

        return true;
    }

    bool ProcessorToolbox::ConfigureProcessors( const scarab::param_node& node )
    {
        for( ProcMapIt iter = fProcMap.begin(); iter != fProcMap.end(); ++iter )
        {
            LDEBUG( proclog, "Attempting to configure processor <" << iter->first << ">" );
            string procName = iter->first;
            string nameUsed(procName);
            if( ! node.has(nameUsed) )
            {
                nameUsed = iter->second.fProc->Name();
                if( ! node.has(nameUsed) )
                {
                    LWARN( proclog, "Did not find a parameter node <" << procName << "> or <" << nameUsed << ">\n"
                            "\tProcessor <" << procName << "> was not configured." );
                    continue;
                }
            }
            const scarab::param_node& subNode = node[nameUsed].as_node();
            try
            {
                iter->second.fProc->Configure(subNode);
            }
            catch( const Exception& e )
            {
                LERROR( proclog, "An error occurred while configuring processor <" << procName << "> with parameter node <" << nameUsed << ">" );
                LERROR( proclog, e.what() );
                return false;
            }
        }
        return true;
    }

    std::shared_ptr< Processor > ProcessorToolbox::GetProcessor( const std::string& procName )
    {
        ProcMapIt it = fProcMap.find( procName );
        if( it == fProcMap.end() )
        {
            LWARN( proclog, "Processor <" << procName << "> was not found." );
            return NULL;
        }
        return it->second.fProc;
    }

    const std::shared_ptr< Processor > ProcessorToolbox::GetProcessor( const std::string& procName ) const
    {
        ProcMapCIt it = fProcMap.find( procName );
        if (it == fProcMap.end())
        {
            LWARN( proclog, "Processor <" << procName << "> was not found." );
            return NULL;
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
            if( newProc == NULL )
            {
                LERROR( proclog, "Unable to create processor of type <" << procType << ">" );
                return false;
            }
            if( ! AddProcessor(procName, newProc) )
            {
                LERROR( proclog, "Unable to add processor <" << procName << ">" );
                //delete newProc;
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
        if( procToRemove == NULL )
        {
            return false;
        }
        //delete procToRemove;
        LDEBUG( proclog, "Processor <" << procName << "> deleted." );
        return true;
    }

    std::shared_ptr< Processor > ProcessorToolbox::ReleaseProcessor( const std::string& procName )
    {
        ProcMapIt it = fProcMap.find( procName );
        if( it == fProcMap.end() )
        {
            LWARN( proclog, "Processor <" << procName << "> was not found." );
            return NULL;
        }
        std::shared_ptr< Processor > procToRelease = it->second.fProc;
        fProcMap.erase( it );
        return procToRelease;
    }

    void ProcessorToolbox::ClearProcessors()
    {
        /*for (ProcMapIt it = fProcMap.begin(); it != fProcMap.end(); it++)
        {
            delete it->second.fProc;
        }*/ //not required for smart pointers
        fProcMap.clear();
        fRunQueue.clear();
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
        catch (boost::exception& e)
        {
            LERROR( proclog, "An error occurred while connecting signals and slots:\n"
                    << "\tSignal " << signalName << " from processor " << signalProcName << " (a.k.a. " << signalProc->Name() << ")" << '\n'
                    << "\tSlot " << slotName << " from processor " << slotProcName << " (a.k.a. " << slotProc->Name() << ")" << '\n'
                    << '\t' << diagnostic_information( e ) );
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
        catch( boost::exception& e )
        {
            LERROR( proclog, "Unable to set breakpoint: " << diagnostic_information( e ) );
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
        catch( boost::exception& e )
        {
            LERROR( proclog, "Unable to set breakpoint: " << diagnostic_information( e ) );
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


    bool ProcessorToolbox::PushBackToRunQueue( const std::string& name )
    {
        ThreadSourceGroup threadGroup;

        if( ! AddProcessorToThreadGroup( name, threadGroup ) )
        {
            LERROR( proclog, "Unable to add processor <" << name << "> to thread group" );
            return false;
        }

        fRunQueue.push_back( threadGroup );

        LINFO( proclog, "Added processor <" << name << "> to the run queue" );
        return true;
    }

    bool ProcessorToolbox::PushBackToRunQueue( std::initializer_list< std::string > names )
    {
        return PushBackToRunQueue( std::vector< std::string >(names) );
    }

    bool ProcessorToolbox::PushBackToRunQueue( std::vector< std::string > names )
    {
        ThreadSourceGroup threadGroup;

        std::stringstream toPrint;
        for( const std::string& name : names )
        {
            if(! AddProcessorToThreadGroup( name, threadGroup ) )
            {
                LERROR( proclog, "Unable to add processor <" << name << "> to thread group" );
                return false;
            }
            toPrint << name << ", "; // the extra comma at the end is removed below
        }

        fRunQueue.push_back( threadGroup );
        std::string toPrintString = toPrint.str();
        toPrintString.resize( toPrintString.size() - 2 );
        LINFO( proclog, "Added processors <" << toPrintString << "> to the run queue" );
        return true;
    }

    bool ProcessorToolbox::AddProcessorToThreadGroup( const std::string& name, ThreadSourceGroup& group )
    {
        std::shared_ptr< Processor > procForRunQueue = GetProcessor( name );
        LDEBUG( proclog, "Attempting to add processor <" << name << "> to the run queue" );
        if( procForRunQueue == nullptr )
        {
            LERROR( proclog, "Unable to find processor <" << name << "> requested for the run queue" );
            return false;
        }

        PrimaryProcessor* primaryProc = dynamic_cast< PrimaryProcessor* >( procForRunQueue.get() );
        if( primaryProc == nullptr )
        {
            LERROR( proclog, "Processor <" << name << "> is not a primary processor." );
            return false;
        }
        //group.insert(primaryProc);
        group.insert( ThreadSource(primaryProc, name) );
        return true;
    }

    void ProcessorToolbox::AsyncRun()
    {
        if( fDoRunThread != nullptr )
        {
            LERROR( proclog, "It appears that a run has already been started" );
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

    void ProcessorToolbox::StartSingleThreadedRun()
    {
        auto singleThreadRun = [&]()
        {
            try
            {
                LPROG( proclog, "Starting single-threaded processing" );

                for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                {
                    for (ThreadGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                    {
                        boost_unique_lock breakContLock( fBreakContMutex );

                        std::string procName( tgIter->fName );
                        LINFO( proclog, "Starting processor <" << procName << ">" );

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
                                    LINFO( proclog, "Thread <" << procName << "> has finished" );
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

                        LINFO( proclog, "Processor <" << procName << "> has finished" );

                        thread.join();
                        fThreadReferences.clear();
                    } // end for loop over the thread group
                } // end for loop over the run-queue

                LPROG( proclog, "Processing is complete (single-threaded)" );
            }
            catch( std::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( proclog, "Caught std::exception thrown in a processor or in the single-threaded run function: " << e.what() );
                LWARN( proclog, "Setting boost::exception of do-run-promise in StartSingleThreadedRun" );
                fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            catch( boost::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( proclog, "Caught boost::exception thrown in a processor or in the single-threaded run function" );
                LWARN( proclog, "Setting boost::exception of do-run-promise in StartSingleThreadedRun" );
                fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            fDoRunPromise.set_value();
            return;
        }; // end single-threaded run lambda

        fDoRunThread = new boost::thread( singleThreadRun );
        return;
    }

    void ProcessorToolbox::StartMultiThreadedRun()
    {
        auto multiThreadRun = [&]()
        {
            try
            {
                LPROG( proclog, "Starting multi-threaded processing" );

                for (RunQueue::iterator rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                {
                    //boost::thread_group threads;
                    std::map< std::thread > threads;

                    { // scope for threadFuturesLock
                        //boost_unique_lock breakContLock( fBreakContMutex );
                        //boost_unique_lock threadFuturesLock( fThreadReferencesMutex );

                        for (ThreadSourceGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                        {
                            std::string procName( tgIter->fName );
                            LINFO( proclog, "Starting processor <" << procName << ">" );

                            threads.emplace( procName, &PrimaryProcessor::operator(), tgIter->fProc );

                            //std::shared_ptr< KTThreadReference > thisThreadRef = std::make_shared< KTThreadReference >();
                            //thisThreadRef->Name() = procName;

                            //thisThreadRef->SetInitiateBreakFunc( [&](){ this->InitiateBreak(); } );
                            //thisThreadRef->SetWaitForContinueFunc( [&]( boost_unique_lock& lock ){ this->WaitForContinue( lock ); } );
                            //fThreadReferences.push_back( thisThreadRef );

                            //boost::condition_variable threadStartedCV;
                            //boost::mutex threadStartedMutex;
                            //bool threadStartedFlag = false;

                            //boost_unique_lock threadStartedLock( threadStartedMutex );
                            //boost::thread* thisThread = new boost::thread( [&](){ tgIter->fProc->operator()( thisThreadRef, threadStartedCV, threadStartedFlag ); } );
                            //KTDEBUG( proclog, "Thread ID is <" << thisThread->get_id() << ">; waiting for thread start" );
                            //while( ! threadStartedFlag )
                            //{
                            //    threadStartedCV.wait( threadStartedLock );
                            //}
                            //KTDEBUG( proclog, "Thread has started" );

                            //threads.add_thread( thisThread );

                        }// end for loop over the thread group
                    } // end scope for threadFuturesLock

                    SharedControl* control = SharedControl::get_instance();
                    bool stillRunning = true;
                    while( stillRunning )
                    {
                        bool breakOrCanceled = control->WaitForBreakOrCanceled();
                        if( breakOrCanceled )
                        {
                            // then at a breakpoint
                            stillRunning = true;
                            LDEBUG( proclog, "At a breakpoint" );
                            // TODO: do we need to do anything while at a breakpoint?
                            if( ! control->WaitToContinue() )
                            {
                                stillRunning = false;
                                LDEBUG( proclog, "Detected cancelation while waiting at breakpoint" );
                            }
                        }
                        else
                        {
                            // then canceled
                            stillRunning = false;
                            LDEBUG( proclog, "Detected cancelation while waiting during running" );
                        }
                    }

                    /*
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
                                LINFO( proclog, "Thread <" << threadName << "> has finished" );
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
                    */
                } // end for loop over the run-queue

                LPROG( proclog, "Processing is complete (multi-threaded)" );
            }
            catch( std::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( proclog, "Caught std::exception thrown in a processor or in the multi-threaded run function: " << e.what() );
                SharedControl::get_instance()->Cancel();
                //LWARN( proclog, "Setting boost::exception of do-run-promise in StartMultiThreadedRun" );
                //fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            catch( boost::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( proclog, "Caught boost::exception thrown in a processor or in the multi-threaded run function" );
                LERROR( proclog, "Diagnostic Information:\n" << diagnostic_information( e ) );
                SharedControl::get_instance()->Cancel();
                //LWARN( proclog, "Setting boost::exception of do-run-promise in StartMultiThreadedRun" );
                //fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            //LWARN( proclog, "Setting value of do-run-promise in StartMultiThreadedRun" );
            //fDoRunPromise.set_value();
            return;
        }; // end multi-threaded run lambda

        fDoRunThread = new std::thread( multiThreadRun );
        return;
    }


    bool ProcessorToolbox::WaitForBreak()
    {
        boost_unique_lock breakContLock( fBreakContMutex );

        if( fDoRunBreakFlag )
        {
            LWARN( proclog, "Can't wait for a break; Already at a breakpoint" );
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
            LERROR( proclog, "An error occurred during processing: " << boost::diagnostic_information( e ) );
            return false;
        }
    }

    void ProcessorToolbox::WaitForEndOfRun()
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

    void ProcessorToolbox::Continue()
    {
        if( ! fDoRunBreakFlag )
        {
            LWARN( proclog, "Not currently at a breakpoint" );
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

        LINFO( proclog, "Continuing from breakpoint" );
        // signal everything to resume
        fDoContinue = true;
        fContinueCV.notify_all();

        return;
    }

    KTDataHandle ProcessorToolbox::GetData( const std::string& threadName )
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
            LWARN( proclog, "Did not find thread <" << threadName << ">" );
            BOOST_THROW_EXCEPTION( KTException() << "Did not find thread <" << threadName << ">" << eom );
        }

        boost_unique_lock lock( (*trIt)->Mutex() );
        return (*trIt)->GetReturnValue();
    }

    void ProcessorToolbox::InitiateBreak()
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
        LWARN( proclog, "Setting value of do-run-promise" );
        fDoRunPromise.set_value();

        return;
    }

    void ProcessorToolbox::CancelThreads()
    {
        boost_unique_lock breakContLock( fBreakContMutex );
        for( auto trIt = fThreadReferences.begin(); trIt != fThreadReferences.end(); ++trIt )
        {
            boost_unique_lock lock( (*trIt)->Mutex() );
            (*trIt)->SetCanceled( true );
        }
        return;
    }

    bool ProcessorToolbox::Run()
    {
        // can throw boost::exception

        AsyncRun();

        WaitForEndOfRun();

        JoinRunThread();

        return true;
    }

} /* namespace Nymph */
