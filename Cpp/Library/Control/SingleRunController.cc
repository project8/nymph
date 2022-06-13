/*
 * SingleRunController.cc
 *
 *  Created on: Jan 13, 2022
 *      Author: N.S. Oblath
 */

#include "SingleRunController.hh"

#include "ControlAccess.hh"
#include "PrimaryProcessor.hh"
#include "QuitChain.hh"

#include "logger.hh"
#include "param.hh"

namespace Nymph
{
    LOGGER( contlog, "SingleRunController");

    SingleRunController::SingleRunController( const ProcessorToolbox& procTB, const std::string& name ) :
            Controller(),
            fProcTB( procTB ),
            fRunQueue(),
            fNActiveThreads( 0 ),
            fDoRunThread(),
            fChainThreads()
    {
    }

    SingleRunController::~SingleRunController()
    {
    }

    void SingleRunController::Configure( const scarab::param_node& node )
    {
        LPROG( contlog, "Configuring run control" );

        Controller::Configure( node );

        // The run queue is an array of processor names, or groups of names, which will be run sequentially.
        // If names are grouped (in another array), those in that group will be run in parallel.
        if( ! node.has("run-queue") )
        {
            LWARN( contlog, "Run queue was not specified" );
        }
        else
        {
            ConfigureRunQueue( node["run-queue"].as_array() );
        }

        return;
    }

    void SingleRunController::ConfigureRunQueue( const scarab::param_array& array )
    {
        for( auto rqIt = array.begin(); rqIt != array.end(); ++rqIt )
        {
            if( rqIt->is_value() )
            {
                if( ! PushBackToRunQueue( (*rqIt)().as_string() ) )
                {
                    THROW_EXCEPT_HERE( ConfigException(array) << "Unable to process run-queue entry: could not add processor to the queue" );
                }
            }
            else if( rqIt->is_array() )
            {
                const scarab::param_array* rqNode = &( rqIt->as_array() );
                std::vector< std::string > names;

                for( scarab::param_array::const_iterator rqArrayIt = rqNode->begin(); rqArrayIt != rqNode->end(); ++rqArrayIt )
                {
                    if( ! rqArrayIt->is_value() )
                    {
                        THROW_EXCEPT_HERE( ConfigException(array) << "Invalid run-queue array entry: not a value" );
                    }
                    names.push_back( (*rqArrayIt)().as_string() );
                }

                if( ! PushBackToRunQueue(names) )
                {
                    THROW_EXCEPT_HERE( ConfigException(array) << "Unable to process run-queue entry: could not add list of processors to the queue" );
                }
            }
            else
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Invalid run-queue entry: not a value or array" );
            }
        }

        return;
    }

    bool SingleRunController::PushBackToRunQueue( const std::string& name )
    {
        ThreadSourceGroupT threadGroup;

        if( ! AddProcessorToThreadGroup( name, threadGroup ) )
        {
            LERROR( contlog, "Unable to add processor <" << name << "> to thread group" );
            return false;
        }

        fRunQueue.push_back( threadGroup );

        LINFO( contlog, "Added processor <" << name << "> to the run queue" );
        return true;
    }

    bool SingleRunController::PushBackToRunQueue( std::initializer_list< std::string > names )
    {
        return PushBackToRunQueue( std::vector< std::string >(names) );
    }

    bool SingleRunController::PushBackToRunQueue( std::vector< std::string > names )
    {
        ThreadSourceGroupT threadGroup;

        std::stringstream toPrint;
        for( const std::string& name : names )
        {
            if(! AddProcessorToThreadGroup( name, threadGroup ) )
            {
                LERROR( contlog, "Unable to add processor <" << name << "> to thread group" );
                return false;
            }
            toPrint << name << ", "; // the extra comma at the end is removed below
        }

        fRunQueue.push_back( threadGroup );
        std::string toPrintString = toPrint.str();
        toPrintString.resize( toPrintString.size() - 2 );
        LINFO( contlog, "Added processors <" << toPrintString << "> to the run queue" );
        return true;
    }

    bool SingleRunController::AddProcessorToThreadGroup( const std::string& name, ThreadSourceGroupT& group )
    {
        std::shared_ptr< Processor > procForRunQueue = fProcTB.GetProcessor( name );
        LDEBUG( contlog, "Attempting to add processor <" << name << "> to the run queue" );
        if( procForRunQueue == nullptr )
        {
            LERROR( contlog, "Unable to find processor <" << name << "> requested for the run queue" );
            return false;
        }

        PrimaryProcessor* primaryProc = dynamic_cast< PrimaryProcessor* >( procForRunQueue.get() );
        if( primaryProc == nullptr )
        {
            LERROR( contlog, "Processor <" << name << "> is not a primary processor." );
            return false;
        }
        group.insert( ThreadSource(primaryProc, name) );
        return true;
    }
    
    void SingleRunController::Run()
    {
        StartRun();

        if( fDoRunThread.joinable() ) 
        {
            WaitForEndOfRun();

            JoinRunThread();
        }

        return;
    }

    void SingleRunController::StartRun()
    {
        if( fDoRunThread.joinable() )
        {
            LERROR( contlog, "It appears that a run has already been started" );
            return;
        }

        if( ! fChainThreads.empty() )
        {
            LERROR( contlog, "Chain threads map is not empty; cannot start new threads" );
            return;
        }

        auto multiThreadRun = [&]()
        {
            try
            {
                LPROG( contlog, "Starting multi-threaded processing" );

                for (auto rqIter = fRunQueue.begin(); rqIter != fRunQueue.end(); ++rqIter)
                {
                    { // scope for mutex lock
                        std::unique_lock< std::mutex > lock( fMutex );

                        // iterate over primary processors in this group and launch threads
                        for( auto tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter )
                        {
                            std::string procName( tgIter->fName );
                            LINFO( contlog, "Starting processor <" << procName << ">" );

                            fChainThreads.emplace( 
                                procName, 
                                ThreadBundle( 
                                    tgIter->fProc, 
                                    std::thread(&PrimaryProcessor::operator(), tgIter->fProc),
                                    std::exception_ptr() 
                                ) 
                            );
                            ++fNActiveThreads;
                        }// end for loop over the thread group
                    }

                    // wait here while things are still running
                    while( ! is_canceled() && fNActiveThreads > 0 )
                    {
                        std::this_thread::sleep_for( std::chrono::milliseconds(fCycleTimeMS) );
                    }
                    LDEBUG( contlog, "Past the wait for threads to stop.\nEither canceled (" << is_canceled() << ") or no active threads remain (# active threads: " << fNActiveThreads << ")" );

                    // ok, we're not running anymore.  join all threads and move along.

                    // join threads and check for exceptions
                    // break out of the run-queue iteration loop if there was an exception
                    bool quitAfterThis = is_canceled();
                    for( auto threadIt = fChainThreads.begin(); threadIt != fChainThreads.end(); ++threadIt )
                    {
                        LINFO( contlog, "Cleaning up thread for processor <" << threadIt->first << ">" )

                        std::thread thread( std::move(std::get<1>( threadIt->second )) );
                        if( ! thread.joinable() )
                        {
                            LDEBUG( contlog, "Thread for processor <" << threadIt->first << "> is not joinable" );
                        }
                        else
                        {
                            LDEBUG( contlog, "Joining thread, ID = " << thread.get_id() );
                            thread.join();
                        }

                        std::exception_ptr exPtr( std::move(std::get<2>( threadIt->second )) );
                        if( exPtr )
                        {
                            try
                            {
                                std::rethrow_exception( exPtr );
                            }
                            catch( const QuitChain& e )
                            {
                                // not necessarily an error, so don't set quitAfterThis to true
                                LINFO( contlog, "Thread had exited with QuitChain" );
                            }
                            catch( const scarab::base_exception& e )
                            {
                                // this is an error, so set quitAfterThis to true
                                LERROR( contlog, "Thread had exited with an exception" );
                                PrintException( e );
                                quitAfterThis = true;
                            }
                        }
                        else
                        {
                            // this thread did not have an exeption set, so it exited normally
                            LINFO( contlog, "Thread had exited normally" )
                        }

                    }

                    fChainThreads.clear();

                    if( quitAfterThis )
                    {
                        LWARN( contlog, "Exiting from run-queue loop under abnormal conditions" );
                        break;
                    }

                } // end for loop over the run-queue

                // we have to cancel on success to make sure anything waiting on the control in some way finishes up
                this->Cancel( RETURN_SUCCESS );

                LPROG( contlog, "Processing is complete" );
            }
            catch( scarab::base_exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( contlog, "Caught scarab::base_exception thrown in a processor or in the multi-threaded run function" );
                LERROR( contlog, "Diagnostic Information:\n" );
                PrintException( e );
                ControlAccess::get_instance()->Cancel();
                return;
            }
            catch( std::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( contlog, "Caught std::exception thrown in a processor or in the multi-threaded run function: " << e.what() );
                ControlAccess::get_instance()->Cancel();
                return;
            }
            return;
        }; // end multi-threaded run lambda

        fDoRunThread = std::thread( multiThreadRun );
        return;
    }

    void SingleRunController::ChainIsQuitting( const std::string& name, std::exception_ptr ePtr )
    {
        Controller::ChainIsQuitting( name, ePtr );

        std::get<2>( fChainThreads.at(name) ) = std::move(ePtr); 

        --fNActiveThreads;

        return;
    }

} /* namespace Nymph */
