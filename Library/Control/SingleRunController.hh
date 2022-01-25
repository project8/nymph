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
     @class ProcessorToolbox
     @author N. S. Oblath

     @brief Manages processors requested by the user at run time.

     @details
     ProcessorToolbox allows the user to setup an application at runtime.

     The user chooses the processors to be used, how they're linked with signals and slots, and what is used to "run"
     the program via a configuration file.

     While this does result in longer configuration files, it drastically simplifies the space of executables that are needed.

     The following order should be used for configuring the processor toolbox:
     <ol>
         <li>Create processors</li>
         <li>Connect signals and slots</li>
         <li>Create the run queue</li>
     </ol>

     Available (nested) configuration values:
     <ul>
         <li>run-single-threaded (bool) -- specify whether to run in single-threaded mode (will be ignored if the application has been compiled with the SINGLETHREADED flag set)
         <li>processors (array of objects) -- create a processor; each object in the array should consist of:
             <ul>
                 <li>type -- string specifying the processor type (matches the string given to the Registrar, which should be specified before the class implementation in each processor's .cc file).</li>
                 <li>name -- string giving the individual processor a name so that multiple processors of the same type can be created.</li>
             </ul>
         </li>
         <li>connection (array of objects) -- connect a signal to a slot; each object should consist of:
             <ul>
                 <li>signal -- <i>proc-name:signal-name</i>; name (i.e. the name given in the array of processors above) of the processor, and the signal that will be emitted.</li>
                 <li>slot -- <i>proc-name:slot-name</li>; name of the processor with the slot that will receive the signal.</li>
                 <li>group-order -- (optional) integer specifying the order in which slots should be called.
             </ul>
         </li>
         <li>run-queue -- (array of strings and arrays of strings) define the queue of processors that will control the running of Nymph.
         The elements of this array specify processors that are run sequentially.
         If an element is itself an array, those processors listed in the sub-array will be run in parallel.
             <ul>
                 <li>processor name -- add a processor to the run queue, or </li>
                 <li>array of processor names -- add a group of processors to the run queue.</li>
             </ul>
             In single-threaded mode, all processors will be run sequentially, in the order specified.
         </li>
     </ul>
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
            void Run( const ProcessorToolbox& procTB );

            void JoinRunThread();

            /// Notify the control that a chain is quitting
            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() );

            // TODO: return value access

            MEMVAR( unsigned, NActiveThreads );

        protected:
            void StartMultiThreadedRun( const ProcessorToolbox& procTB );

            std::thread fDoRunThread;

            typedef std::tuple< PrimaryProcessor*, std::thread, std::exception_ptr > ThreadBundle;
            std::map< std::string, ThreadBundle > fChainThreads;

//            void do_cancellation( int code );

        public:
        /*
            template< typename... Args >
            std::tuple< Args&... >& GetReturn();

            template< typename... Args >
            void Break( Args&... args ); // to be called by a working thread
        */

            // WARNING: This function resets the state of this class to a just-constructed state
            //          It is intended for use with testing when a single application might use it multiple times
            void Reset();

    };

   inline void SingleRunController::JoinRunThread()
    {
        fDoRunThread.join();
        return;
    }


} /* namespace Nymph */

#endif /* NYMPH_SINGLERUNCONTROLLER_HH_ */
