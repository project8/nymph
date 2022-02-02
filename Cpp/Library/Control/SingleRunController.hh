/**
 @file SingleRunController.hh
 @brief Contains SingleRunController
 @details 
 @author: N. S. Oblath
 @date: Sep 27, 2012
 */

#ifndef NYMPH_SINGLERUNCONTROLLER_HH_
#define NYMPH_SINGLERUNCONTROLLER_HH_

#include "Controller.hh"

#include <map>
#include <thread>
#include <tuple>

namespace scarab
{
    class param_node;
}

namespace Nymph
{
    class PrimaryProcessor;
    class ProcessorToolbox;

    /*!
     @class SingleRunController
     @author N. S. Oblath

     @brief Performs a single run as specified to the ProcessorToolbox

    */
    class SingleRunController : public Controller
    {
        public:
            SingleRunController( const std::string& name = "single-run-controller" );
            virtual ~SingleRunController();

        public:
            /// Configure the controller
            void Configure( const scarab::param_node& node );

        public:
            /// Process the run queue.
            /// This will call Run() on all of the primary processors in the queue.
            /// This is a blocking call that waits until the run is completed before returning.
            void Run( const ProcessorToolbox& procTB );

            /// Asynchronously start a run
            /// Starts run in a new thread and returns
            void StartRun( const ProcessorToolbox& procTB );

            /// Wait for the run to complete; 
            void JoinRunThread();

            /// Notify the control that a chain is quitting
            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() );

            // TODO: return value access

            MEMVAR( unsigned, NActiveThreads );

        protected:
            std::thread fDoRunThread;

            typedef std::tuple< PrimaryProcessor*, std::thread, std::exception_ptr > ThreadBundle;
            std::map< std::string, ThreadBundle > fChainThreads;

        public:
        /*
            template< typename... Args >
            std::tuple< Args&... >& GetReturn();

            template< typename... Args >
            void Break( Args&... args ); // to be called by a working thread
        */

    };

   inline void SingleRunController::JoinRunThread()
    {
        fDoRunThread.join();
        return;
    }


} /* namespace Nymph */

#endif /* NYMPH_SINGLERUNCONTROLLER_HH_ */
