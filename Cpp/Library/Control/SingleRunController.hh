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

#include "ProcessorToolbox.hh"

#include <deque>
#include <initializer_list>
#include <map>
#include <set>
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
            SingleRunController( const ProcessorToolbox& procTB, const std::string& name = "single-run-controller" );
            virtual ~SingleRunController();

        public:
            /// Configure the controller
            void Configure( const scarab::param_node& node );

        public:
            struct ThreadSource
            {
                PrimaryProcessor* fProc;
                std::string fName;
                //ControlAccessPtr fControlAccess;
                ThreadSource( PrimaryProcessor* proc, const std::string& name ) : 
                        fProc(proc), fName(name)//, fControlAccess( new ControlAccess() )
                {}
            };
            struct CompareThreadSource
            {
                bool operator()( const ThreadSource& lhs, const ThreadSource& rhs ) const
                {
                    return lhs.fProc < rhs.fProc;
                }
            };
            typedef std::set< ThreadSource, CompareThreadSource > ThreadSourceGroupT;
            typedef std::deque< ThreadSourceGroupT > RunQueueT;

            /// Setup the run queue according to the `run-queue` configuration block
            void ConfigureRunQueue( const scarab::param_array& node );

            /// Push a single processor to the back of the run queue
            bool PushBackToRunQueue( const std::string& name );

            /// Push a set of processors to be run in parallel to the back of the run queue
            bool PushBackToRunQueue( std::initializer_list< std::string > names );
            /// Push a set of processors to be run in parallel to the back of the run queue
            bool PushBackToRunQueue( std::vector< std::string > names );

            /// Remove the last item in the run queue, whether it's a single processor or a group of processors
            void PopBackOfRunQueue();

            /// Clear the run queue
            void ClearRunQueue();

            /// Const access to the run queue
            const RunQueueT& RunQueue() const;

        protected:
            const ProcessorToolbox& fProcTB;

            RunQueueT fRunQueue;

            bool AddProcessorToThreadGroup( const std::string& name, ThreadSourceGroupT& group );

        public:
            /// Process the run queue.
            /// This will call Run() on all of the primary processors in the queue.
            /// This is a blocking call that waits until the run is completed before returning.
            void Run();

            /// Asynchronously start a run
            /// Starts run in a new thread and returns
            void StartRun();

            /// Wait for the run to complete; 
            void JoinRunThread();

            /// Notify the control that a chain is quitting
            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() );

            MEMVAR( unsigned, NActiveThreads );

        protected:
            std::thread fDoRunThread;

            typedef std::tuple< PrimaryProcessor*, std::thread, std::exception_ptr > ThreadBundle;
            std::map< std::string, ThreadBundle > fChainThreads;

    };

    inline void SingleRunController::PopBackOfRunQueue()
    {
        fRunQueue.pop_back();
        return;
    }

    inline void SingleRunController::ClearRunQueue()
    {
        fRunQueue.clear();
        return;
    }

    inline const SingleRunController::RunQueueT& SingleRunController::RunQueue() const
    {
        return fRunQueue;
    }

   inline void SingleRunController::JoinRunThread()
    {
        fDoRunThread.join();
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_SINGLERUNCONTROLLER_HH_ */
