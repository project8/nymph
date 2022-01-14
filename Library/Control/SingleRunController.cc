/*
 * SingleRunController.cc
 *
 *  Created on: Jan 13, 2022
 *      Author: N.S. Oblath
 */

#include "SingleRunController.hh"

#include "logger.hh"

#include <map>
#include <thread>


namespace Nymph
{
    LOGGER( contlog, "SingleRunController");

    SingleRunController::SingleRunController( const std::string& name ) :
            fDoRunThread(),
            fMutex(),
            fCondVarContinue(),
            fCondVarBreak(),
            fBreakFlag( false ),
            fCanceledFlag( false ),
            fCycleTimeMS( 500 ),
            fNActiveThreads( 0 )
    {
    }

    SingleRunController::~SingleRunController()
    {
    }

    void ProcessorToolbox::Configure( const scarab::param_node& node )
    {
        LPROG( proclog, "Configuring run control" );


    }



    void ProcessorToolbox::AsyncRun()
    {
        if( fDoRunThread.joinable() )
        {
            LERROR( proclog, "It appears that a run has already been started" );
            return;
        }

        StartMultiThreadedRun();

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
                    SharedControl* control = SharedControl::get_instance();

                    typedef std::pair< PrimaryProcessor*, std::thread > ThreadBundle;
                    std::map< std::string, ThreadBundle > threads;

                    // iterate over primary processors in this group and launch threads
                    for (ThreadSourceGroup::iterator tgIter = rqIter->begin(); tgIter != rqIter->end(); ++tgIter)
                    {
                        std::string procName( tgIter->fName );
                        LINFO( proclog, "Starting processor <" << procName << ">" );

                        threads.emplace( procName, ThreadBundle( tgIter->fProc, std::thread(&PrimaryProcessor::operator(), tgIter->fProc) ) );

                        control->IncrementActiveThreads();
                    }// end for loop over the thread group

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

                    // join threads and check for exceptions
                    // break out of the run-queue iteration loop if there was an exception
                    bool quitAfterThis = control->IsCanceled();
                    for( auto threadIt = threads.begin(); threadIt != threads.end(); ++threadIt )
                    {
                        LINFO( proclog, "Cleaning up thread for processor <" << threadIt->first << ">" )
                        LDEBUG( proclog, "Joining thread" );
                        threadIt->second.second.join();
                        if( threadIt->second.first->ExceptionPtr() )
                        {
                            try
                            {
                                std::rethrow_exception( threadIt->second.first->ExceptionPtr() );
                            }
                            catch( const QuitThread& e )
                            {
                                // not necessarily an error, so don't set quitAfterThis to true
                                LINFO( proclog, "Thread exited with QuitThread" );
                            }
                            catch( const scarab::base_exception& e )
                            {
                                // this is an error, so set quitAfterThis to true
                                LERROR( proclog, "Thread exited with an exception" );
                                PrintException( e );
                                quitAfterThis = true;
                            }
                        }
                        else
                        {
                            // this thread did not have an exeption set, so it exited normally
                            LINFO( proclog, "Thread exited normally" )
                        }
                    }

                    if( quitAfterThis )
                    {
                        LWARN( proclog, "Exiting from run-queue loop under abnormal conditions" );
                        break;
                    }

                } // end for loop over the run-queue

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
                //LWARN( proclog, "Setting boost::exception of do-run-promise in StartMultiThreadedRun" );
                //fDoRunPromise.set_exception( boost::current_exception() );
                return;
            }
            return;
        }; // end multi-threaded run lambda

        fDoRunThread = std::thread( multiThreadRun );
        return;
    }


    bool ProcessorToolbox::WaitForBreakOrCanceled()
    {
        return SharedControl::get_instance()->WaitForBreakOrCanceled();
    }

    bool ProcessorToolbox::WaitToContinue()
    {
        return SharedControl::get_instance()->WaitToContinue();
    }

    void ProcessorToolbox::WaitForEndOfRun()
    {
        LDEBUG( proclog, "Waiting for end-of-run" );
        while( WaitForBreakOrCanceled() )
        {
            LDEBUG( proclog, "Reached breakpoint; waiting for continue" );
            WaitToContinue();
            LDEBUG( proclog, "Processing resuming; waiting for end-of-run" );
        }
        LDEBUG( proclog, "End-of-run reached" );

        return;
    }

    void ProcessorToolbox::Continue()
    {
        SharedControl::get_instance()->Resume();
        return;
    }

   void ProcessorToolbox::JoinRunThread()
   {
       fDoRunThread.join();
       return;
   }

    void ProcessorToolbox::CancelThreads()
    {
        SharedControl::get_instance()->Cancel();
        return;
    }

    bool ProcessorToolbox::Run()
    {
        // can throw scarab::base_exception

        AsyncRun();

        WaitForEndOfRun();

        JoinRunThread();

        return true;
    }




        void SharedControl::Reset()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "Reseting ControlAccess" );
        fBreakFlag = false;
        fCanceledFlag = false;
        fCycleTimeMS = 500;
        fNActiveThreads = 0;
        return;
    }

    void SharedControl::Cancel()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "CANCEL called" );
        fCanceledFlag = true;
        fCondVarBreak.notify_all();
        return;
    }

    bool SharedControl::IsAtBreak() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        return fBreakFlag;
    }

    bool SharedControl::IsCanceled() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        return fCanceledFlag;
    }

    bool SharedControl::WaitToContinue() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        if( fBreakFlag )
        {
            while( fBreakFlag && ! fCanceledFlag && fNActiveThreads > 0 )
            {
                fCondVarContinue.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
            }
        }
        return (! fCanceledFlag) && (fNActiveThreads > 0); // returns true if the thread should continue; false if it should end
    }

    bool SharedControl::WaitForBreakOrCanceled() const
    {
        std::unique_lock< std::mutex > lock( fMutex );
        while( ! fBreakFlag && ! fCanceledFlag && fNActiveThreads > 0 )
        {
            fCondVarBreak.wait_for( lock, std::chrono::milliseconds(fCycleTimeMS) );
        }
        return fCanceledFlag || fNActiveThreads == 0 ? false : fBreakFlag;
    }

    void SharedControl::Resume()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        LDEBUG( contlog, "RESUME called" );
        fBreakFlag = false;
        fCondVarContinue.notify_all();
    }

    void SharedControl::IncrementActiveThreads()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        ++fNActiveThreads;
        LDEBUG( contlog, "Incremented active threads: " << fNActiveThreads );
        return;
    }

    void SharedControl::DecrementActiveThreads()
    {
        std::unique_lock< std::mutex > lock( fMutex );
        if( fNActiveThreads > 0 ) --fNActiveThreads;
        LDEBUG( contlog, "Decremented active threads: " << fNActiveThreads );
        return;
    }



} /* namespace Nymph */
