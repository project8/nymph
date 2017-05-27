/**
 @file KTProcessorToolbox.hh
 @brief Contains KTProcessorToolbox
 @details Manages processors requested by the user at run time.
 @author: N. S. Oblath
 @date: Sep 27, 2012
 */

#ifndef KTPROCESSORTOOLBOX_HH_
#define KTPROCESSORTOOLBOX_HH_

#include "KTConfigurable.hh"
#include "KTMemberVariable.hh"
#include "KTThreadReference.hh"

#include "factory.hh"

#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread/future.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <deque>
#include <initializer_list>
#include <limits>
#include <set>


namespace Nymph
{
    class KTPrimaryProcessor;
    class KTProcessor;

    /*!
     @class KTProcessorToolbox
     @author N. S. Oblath

     @brief Manages processors requested by the user at run time.

     @details
     KTProcessorToolbox allows the user to setup an application at runtime.

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
    class KTProcessorToolbox : public KTConfigurable
    {
        private:
            typedef boost::unique_lock< boost::mutex > boost_unique_lock;

        public:
            KTProcessorToolbox(const std::string& name = "processor-toolbox");
            virtual ~KTProcessorToolbox();

        private:
            scarab::factory< KTProcessor, const std::string& >* fProcFactory; // singleton; not owned by KTProcessorToolbox


        public:
            /// Configure the toolbox: create the processors; connnect signals and slots; and setup the run queue.
            bool Configure(const scarab::param_node* node);

            /// Configure processors (only those specified in the toolbox)
            bool ConfigureProcessors(const scarab::param_node* node);
            /// Configure processors from a json-encoding of their configurations
            bool ConfigureProcessors(const std::string& config);

            MEMBERVARIABLE( bool, RunSingleThreaded );

        private:
            struct ProcessorInfo
            {
                KTProcessor* fProc;
            };
            typedef std::map< std::string, ProcessorInfo > ProcessorMap;
            typedef ProcessorMap::iterator ProcMapIt;
            typedef ProcessorMap::const_iterator ProcMapCIt;
            typedef ProcessorMap::value_type ProcMapValue;

        public:
            /// Get a pointer to a processor in the toolbox
            KTProcessor* GetProcessor(const std::string& procName);
            /// Get a pointer to a processor in the toolbox
            const KTProcessor* GetProcessor(const std::string& procName) const;

            /// Add a processor to the toolbox
            /// Toolbox takes ownership of the processor
            bool AddProcessor(const std::string& procName, KTProcessor* proc);
            bool AddProcessor(const std::string& procType, const std::string& procName);

            /// Remove a processor from the toolbox
            bool RemoveProcessor(const std::string& procName);

            /// Remove a processor from the toolbox and return it to the user
            /// Ownership is passed to the user
            KTProcessor* ReleaseProcessor(const std::string& procName);

            /// Remove all processors from the toolbox
            /// Also clears the run queue
            void ClearProcessors();

        private:
            ProcessorMap fProcMap;


        public:
            /// Make a connection between the signal from one processor and the slot from another processor
            /// Both processors should already have been added to the Toolbox
            /// Signal and slot strings should be formatted as: [processor name]:[signal/slot name]
            bool MakeConnection(const std::string& signal, const std::string& slot, int order = std::numeric_limits< int >::min());

            /// Make a connection between the signal from one processor and the slot from another processor
            /// Both processors should already have been added to the Toolbox
            bool MakeConnection(const std::string& signalProcName, const std::string& signalName, const std::string& slotProcName, const std::string& slotName, int order = std::numeric_limits< int >::min());

            /// Set a breakpoint on a slot
            /// Slot string should be formatted as: [processor name]:[slot name]
            bool SetBreakpoint(const std::string& slot);
            /// Set a breakpoint on a slot
            bool SetBreakpoint(const std::string& slotProcName, const std::string& slotName);

            /// Remove a breakpoint from a slot
            /// Slot string should be formatted as: [processor name]:[slot name]
            bool RemoveBreakpoint(const std::string& slot);
            /// Remove a breakpoint from a slot
            bool RemoveBreakpoint(const std::string& slotProcName, const std::string& slotName);

        private:
            bool ParseSignalSlotName(const std::string& toParse, std::string& nameOfProc, std::string& nameOfSigSlot) const;
            static const char fSigSlotNameSep = ':';


        public:
            /// Push a single processor to the back of the run queue
            bool PushBackToRunQueue(const std::string& name);

            /// Push a set of processors to be run in parallel to the back of the run queue
            bool PushBackToRunQueue(std::initializer_list< std::string > names);
            /// Push a set of processors to be run in parallel to the back of the run queue
            bool PushBackToRunQueue(std::vector< std::string > names);

            /// Remove the last item in the run queue, whether it's a single processor or a group of processors
            void PopBackOfRunQueue();

            /// Clear the run queue
            void ClearRunQueue();

        private:
            struct Thread
            {
                    KTPrimaryProcessor* fProc;
                    std::string fName;
                    Thread(KTPrimaryProcessor* proc, const std::string& name) : fProc(proc), fName(name)
                    {}
            };
            struct CompareThread
            {
                bool operator() (const Thread& lhs, const Thread& rhs)
                {
                    return lhs.fProc < rhs.fProc;
                }
            };
            typedef std::set< Thread, CompareThread > ThreadGroup;
            typedef std::deque< ThreadGroup > RunQueue;
            bool AddProcessorToThreadGroup(const std::string& name, ThreadGroup& group);

            RunQueue fRunQueue;

        public:
            /// Process the run queue.
            /// This will call Run() on all of the processors in the queue.
            bool Run();

            void AsyncRun();

            void WaitForContinue( boost_unique_lock& lock );

            /// Returns when processing is completed or a breakpoint is reached
            /// Throws a boost::exception if there's an error with the future object in use
            /// If the return is true, processing can continue after the break
            /// If the return is false, processing has ended (either normally or due to an error)
            bool WaitForBreak();

            void WaitForEndOfRun();

            void Continue();

            void CancelThreads();

            void JoinRunThread();

            KTDataPtr GetData( const std::string& threadName );

        private:
            friend class KTThreadReference;

            typedef boost::shared_future< KTDataPtr > Future;

            void StartSingleThreadedRun();
            void StartMultiThreadedRun();

            // called from KTThreadReference::Break
            void InitiateBreak();

            std::vector< std::shared_ptr< KTThreadReference > > fThreadReferences;

            boost::condition_variable fContinueCV;
            bool fDoContinue;
            boost::mutex fBreakContMutex;

            boost::thread* fDoRunThread;
            boost::promise< void > fDoRunPromise;
            boost::shared_future< void > fDoRunFuture;
            bool fDoRunBreakFlag;

    };

    template< class Value, class IIterator >
    class KTThreadRefFutureIter : public boost::iterator_adaptor< KTThreadRefFutureIter< Value, IIterator >, IIterator, Value, boost::random_access_traversal_tag >
    {
        private:
            // used for the conversion constructor below
            struct enabler {};

        public:
            KTThreadRefFutureIter() :
                    KTThreadRefFutureIter::iterator_adaptor_()
            {}
            KTThreadRefFutureIter( const IIterator& other ) :
                    KTThreadRefFutureIter::iterator_adaptor_( other )
            {}

            // converts from Iterator to ConstIterator, but the enable_if business prevents converting from ConstIterator to Iterator
            template< class OtherValue, class OtherIIterator >
            KTThreadRefFutureIter( const KTThreadRefFutureIter< OtherValue, OtherIIterator > & other, typename boost::enable_if< boost::is_convertible< OtherValue, Value >, enabler >::type = enabler() ) :
                    KTThreadRefFutureIter::iterator_adaptor_( other.base )
            {}

        private:
            friend class boost::iterator_core_access;

            Value& dereference() const
            {
                return (*(this->base_reference()))->GetDataPtrRetFuture();
            }
    };

    typedef KTThreadRefFutureIter< boost::unique_future< KTDataPtr >, std::vector< std::shared_ptr< KTThreadReference > >::iterator > KTThreadRefFutureIterator;
    typedef KTThreadRefFutureIter< const boost::unique_future< KTDataPtr >, std::vector< std::shared_ptr< KTThreadReference > >::const_iterator > KTThreadRefFutureConstIterator;


    inline void KTProcessorToolbox::PopBackOfRunQueue()
    {
        fRunQueue.pop_back();
        return;
    }

    inline void KTProcessorToolbox::ClearRunQueue()
    {
        fRunQueue.clear();
        return;
    }

    inline void KTProcessorToolbox::WaitForContinue( boost_unique_lock& lock )
    {
        //fMasterContSignal.wait();
        while( ! fDoContinue )
        {
            fContinueCV.wait( lock );
        }
        return;
    }

    inline void KTProcessorToolbox::JoinRunThread()
    {
        if( fDoRunThread == nullptr ) return;
        fDoRunThread->join();
        delete fDoRunThread;
        fDoRunThread = nullptr;
    }

} /* namespace Nymph */
#endif /* KTPROCESSORTOOLBOX_HH_ */
