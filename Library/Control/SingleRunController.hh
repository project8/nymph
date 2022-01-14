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

#include "ControlAccess.hh"
#include "ProcessorToolbox.hh"

#include <deque>
#include <initializer_list>
#include <limits>
#include <set>
#include <memory>
#include <thread>


namespace Nymph
{
    class PrimaryProcessor;
    class Processor;

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
            ProcessorToolbox( const std::string& name = "processor-toolbox" );
            virtual ~ProcessorToolbox();

        public:
            /// Configure the controller
            void Configure( const scarab::param_node& node );

        public:
            /// Process the run queue.
            /// This will call Run() on all of the primary processors in the queue.
            bool Run();

            void AsyncRun();

            bool WaitToContinue();

            /// Returns when processing is completed or a breakpoint is reached
            /// Throws a boost::exception if there's an error with the future object in use
            /// If the return is true, processing can continue after the break
            /// If the return is false, processing has ended (either normally or due to an error)
            bool WaitForBreakOrCanceled();

            void WaitForEndOfRun();

            void Continue();

            void CancelThreads();

            void JoinRunThread();

            // TODO: return value access

        protected:
            void StartMultiThreadedRun();

            std::thread fDoRunThread;


        public:
            template< typename... Args >
            std::tuple< Args&... >& GetReturn();

            template< typename... Args >
            void Break( Args&... args ); // to be called by a working thread

            void Cancel(); // to be called by a working thread

            bool IsAtBreak() const; // to be called by a working thread

            bool IsCanceled() const; // to be called by a working thread

            // return: true = continue; false = cancel
            bool WaitToContinue() const; // to be called by a working thread

            // return: true = was a break; false = cancel
            bool WaitForBreakOrCanceled() const; // to be called by the processor toolbox

            void Resume(); // to be called by the processor toolbox

            void IncrementActiveThreads();
            void DecrementActiveThreads();

            MEMVAR_REF_MUTABLE( std::mutex, Mutex );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarContinue );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarBreak );
            MEMVAR_NOSET( bool, BreakFlag );
            MEMVAR_NOSET( bool, CanceledFlag );
            MEMVAR( unsigned, CycleTimeMS );
            MEMVAR_SHARED_PTR_CONST( ReturnBufferBase, ReturnPtr );
            MEMVAR( unsigned, NActiveThreads );

            // WARNING: This function resets the state of this class to a just-constructed state
            //          It is intended for use with testing when a single application might use it multiple times
            void Reset();

    };

    template< typename... Args >
    void SharedControl::Break( Args&... args )
    {
        while( IsAtBreak() && ! IsCanceled() )
        {
            if( ! WaitToContinue() )
            {
                THROW_EXCEPT_HERE( Exception() << "Canceled while waiting to initiate a breakpoint" );
            }
        }

        std::unique_lock< std::mutex > lock( fMutex );
        fBreakFlag = true;
        fCondVarBreak.notify_all();
        return;
    }



    template< typename... Args >
    std::tuple< Args&... >& SharedControl::GetReturn()
    {
        if( ! fReturnPtr ) THROW_EXCEPT_HERE( Exception() << "No return available" );
        std::unique_lock< std::mutex > lock( fMutex );
        std::shared_ptr< ReturnBuffer< Args... > > buffer( std::dynamic_pointer_cast< ReturnBuffer< Args... > >(fReturnPtr) );
        if( buffer == nullptr ) THROW_EXCEPT_HERE( Exception() << "Incorrect types used to get return" );
        return buffer->GetReturn();
    }


} /* namespace Nymph */

#endif /* NYMPH_SINGLERUNCONTROLLER_HH_ */
