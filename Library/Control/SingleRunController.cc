/*
 * SingleRunController.cc
 *
 *  Created on: Jan 13, 2022
 *      Author: N.S. Oblath
 */

#include "SingleRunController.hh"

#include "logger.hh"

namespace Nymph
{
    LOGGER( contlog, "SingleRunController");

    SingleRunController::SingleRunController( const std::string& name ) :
    //        fNActiveThreads( 0 ),
            fDoRunThread(),
            fChainThreads()
    {
    }

    SingleRunController::~SingleRunController()
    {
    }

    void SingleRunController::Configure( const scarab::param_node& node )
    {
        LPROG( proclog, "Configuring run control" );


    }

    bool SingleRunController::Run( const ProcessorToolbox& procTB )
    {
        // can throw scarab::base_exception

        StartMultiThreadedRun( procTB );

        WaitForEndOfRun();

        JoinRunThread();

        return true;
    }

    void SingleRunController::StartMultiThreadedRun( const ProcessorToolbox& procTB )
    {
        if( fDoRunThread.joinable() )
        {
            LERROR( proclog, "It appears that a run has already been started" );
            return;
        }

        auto multiThreadRun = [&]()
        {
            try
            {
                LPROG( proclog, "Starting multi-threaded processing" );

                for (auto rqIter = procTB.RunQueue().begin(); rqIter != procTB.RunQueue().begin().end(); ++rqIter)
                {
                    { // scope for mutex lock
                        std::unique_lock< std::mutex > lock( fMutex );

                        // iterate over primary processors in this group and launch threads
                        for( auto tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter )
                        {
                            std::string procName( tgIter->fName );
                            LINFO( proclog, "Starting processor <" << procName << ">" );

                            fChainThreads.emplace( 
                                procName, 
                                ThreadBundle( 
                                    tgIter->fProc, 
                                    std::thread(&PrimaryProcessor::operator(), tgIter->fProc),
                                    std::exception_ptr() 
                                ) 
                            );
//                            ++fNActiveThreads;
                        }// end for loop over the thread group
                    }

                    // wait here while things are still running
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

                    // ok, we're not running anymore.  join all threads and move along.

                    // join threads and check for exceptions
                    // break out of the run-queue iteration loop if there was an exception
                    bool quitAfterThis = is_canceled();
                    for( auto threadIt = threads.begin(); threadIt != threads.end(); ++threadIt )
                    {
                        LINFO( proclog, "Cleaning up thread for processor <" << threadIt->first << ">" )

                        std::thread thread( std::move(std::get<1>( threadIt->second )) );
                        if( ! thread.joinable() )
                        {
                            LDEBUG( contlog, "Thread for processor <" << threadIt->first << "> is not joinable" );
                        }
                        else
                        {
                            LDEBUG( proclog, "Joining thread, ID = " << thread.thread_id() );
                            thread.join();
                        }

                        std::exception_ptr exPtr( std::move(std::get<2>( threadIt->second )) );
                        if( exPtr )
                        {
                            try
                            {
                                std::rethrow_exception( exPtr );
                            }
                            catch( const QuitThread& e )
                            {
                                // not necessarily an error, so don't set quitAfterThis to true
                                LINFO( proclog, "Thread had exited with QuitThread" );
                            }
                            catch( const scarab::base_exception& e )
                            {
                                // this is an error, so set quitAfterThis to true
                                LERROR( proclog, "Thread had exited with an exception" );
                                PrintException( e );
                                quitAfterThis = true;
                            }
                        }
                        else
                        {
                            // this thread did not have an exeption set, so it exited normally
                            LINFO( proclog, "Thread had exited normally" )
                        }
                    }

                    if( quitAfterThis )
                    {
                        LWARN( proclog, "Exiting from run-queue loop under abnormal conditions" );
                        break;
                    }

                } // end for loop over the run-queue

                // we have to cancel on success to make sure anything waiting on the control in some way finishes up
                this->Cancel( RETURN_SUCCESS );

                LPROG( proclog, "Processing is complete" );
            }
            catch( scarab::base_exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( proclog, "Caught scarab::base_exception thrown in a processor or in the multi-threaded run function" );
                LERROR( proclog, "Diagnostic Information:\n" );
                PrintException( e );
                SharedControl::get_instance()->Cancel();
                return;
            }
            catch( std::exception& e )
            {
                // exceptions thrown in this function or from within processors will end up here
                LERROR( proclog, "Caught std::exception thrown in a processor or in the multi-threaded run function: " << e.what() );
                SharedControl::get_instance()->Cancel();
                return;
            }
            return;
        }; // end multi-threaded run lambda

        fDoRunThread = std::thread( multiThreadRun );
        return;
    }

    void SingleRunController::ChainIsQuitting( const std::string& name, std::exception_ptr ePtr )
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "Chain <" << name << "> is quitting" );
        
        if( ePtr )
        {
            try
            {
                std::rethrow_exception( ePtr );
            }
            catch( const QuitChain& e )
            {
                // not necessarily an error, so don't set quitAfterThis to true
                LINFO( contlog, "Chain <" << name << "> exited with QuitChain" );
            }
            catch( const scarab::base_exception& e )
            {
                // this is an error, so set quitAfterThis to true
                LERROR( contlog, "Chain <" << name << "> exited with an exception" );
                PrintException( e );
                this->Cancel( RETURN_ERROR );
            }
        }

//        --fNActiveThreads;

        return;
    }

   void SingleRunController::JoinRunThread()
   {
       fDoRunThread.join();
       return;
   }



    void SharedControl::Reset()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "Reseting ControlAccess" );
        fBreakFlag = false;
        fCanceledFlag = false;
        fCycleTimeMS = 500;
//        fNActiveThreads = 0;
        return;
    }





} /* namespace Nymph */
