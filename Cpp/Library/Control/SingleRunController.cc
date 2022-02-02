/*
 * SingleRunController.cc
 *
 *  Created on: Jan 13, 2022
 *      Author: N.S. Oblath
 */

#include "SingleRunController.hh"

#include "ControlAccess.hh"
#include "PrimaryProcessor.hh"
#include "ProcessorToolbox.hh"
#include "QuitChain.hh"

#include "logger.hh"
#include "param.hh"

namespace Nymph
{
    LOGGER( contlog, "SingleRunController");

    SingleRunController::SingleRunController( const std::string& name ) :
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

        return;
    }

    void SingleRunController::Run( const ProcessorToolbox& procTB )
    {
        StartRun( procTB );

        if( fDoRunThread.joinable() ) 
        {
            WaitForEndOfRun();

            JoinRunThread();
        }

        return;
    }

    void SingleRunController::StartRun( const ProcessorToolbox& procTB )
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

                for (auto rqIter = procTB.RunQueue().begin(); rqIter != procTB.RunQueue().end(); ++rqIter)
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

        --fNActiveThreads;

        return;
    }

} /* namespace Nymph */
