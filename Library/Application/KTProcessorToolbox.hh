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
//#include "KTNOFactory.hh"

#include "factory.hh"

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


        public:
            /// Process the run queue.
            /// This will call Run() on all of the processors in the queue.
            bool Run();


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
            //typedef std::set< KTPrimaryProcessor* > ThreadGroup;
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

    };

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



} /* namespace Nymph */
#endif /* KTPROCESSORTOOLBOX_HH_ */
