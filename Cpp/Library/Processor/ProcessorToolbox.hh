/**
 @file ProcessorToolbox.hh
 @brief Contains ProcessorToolbox
 @details Manages processors requested by the user at run time.
 @author: N. S. Oblath
 @date: Sep 27, 2012
 */

#ifndef NYMPH_PROCESSORTOOLBOX_HH_
#define NYMPH_PROCESSORTOOLBOX_HH_

#include "ControlAccess.hh"
#include "MemberVariable.hh"

#include "factory.hh"
#include "param.hh"

#include <limits>
#include <set>
#include <memory>


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
    */
    class ProcessorToolbox
    {
        protected:
            typedef std::unique_lock< std::mutex > unique_lock;

        public:
            ProcessorToolbox( const std::string& name = "processor-toolbox" );
            virtual ~ProcessorToolbox();

        protected:
            scarab::factory< Processor, const std::string& >* fProcFactory; // singleton; not owned by ProcessorToolbox


        public:
            /// Configure the toolbox: create the processors; connnect signals and slots; and setup the run queue.
            void Configure( const scarab::param_node& node );

        protected:
            struct ProcessorInfo
            {
                std::shared_ptr< Processor > fProc;
            };
            typedef std::map< std::string, ProcessorInfo > ProcessorMap;
            typedef ProcessorMap::iterator ProcMapIt;
            typedef ProcessorMap::const_iterator ProcMapCIt;
            typedef ProcessorMap::value_type ProcMapValue;

        public:
            /// Create processors and configure each according to the `processors` configuration block
            void ConfigureProcessors( const scarab::param_array& node );

            /// Get a pointer to a processor in the toolbox
            std::shared_ptr< Processor > GetProcessor( const std::string& procName );
            /// Get a pointer to a processor in the toolbox
            const std::shared_ptr< Processor > GetProcessor( const std::string& procName ) const;

            /// Add a processor to the toolbox
            /// Toolbox takes ownership of the processor
            bool AddProcessor( const std::string& procName, std::shared_ptr< Processor > proc );
            bool AddProcessor( const std::string& procType, const std::string& procName );

            /// Remove a processor from the toolbox
            bool RemoveProcessor( const std::string& procName );

            /// Remove a processor from the toolbox and return it to the user
            /// Ownership is passed to the user
            std::shared_ptr< Processor > ReleaseProcessor( const std::string& procName );

            /// Remove all processors from the toolbox
            /// Also clears the run queue
            void ClearProcessors();

        protected:
            ProcessorMap fProcMap;


        public:
            // for the MakeConnection overloading, extra overloading is used instead of default parameters so that the python interface works

            /// Make connections between processors according to the `connections` configuration block
            void ConfigureConnections( const scarab::param_array& node );

            /// Make a connection between the signal from one processor and the slot from another processor
            /// Both processors should already have been added to the Toolbox
            /// Signal and slot strings should be formatted as: [processor name]:[signal/slot name]
            bool MakeConnection(const std::string& signal, const std::string& slot, int order);
            bool MakeConnection(const std::string& signal, const std::string& slot);

            /// Make a connection between the signal from one processor and the slot from another processor
            /// Both processors should already have been added to the Toolbox
            bool MakeConnection(const std::string& signalProcName, const std::string& signalName, const std::string& slotProcName, const std::string& slotName, int order);
            bool MakeConnection(const std::string& signalProcName, const std::string& signalName, const std::string& slotProcName, const std::string& slotName);

            /// Set a breakpoint on a signal
            /// Slot string should be formatted as: [processor name]:[signal name]
            bool SetBreakpoint( const std::string& signal );
            /// Set a breakpoint on a signal
            bool SetBreakpoint( const std::string& signalProcName, const std::string& signalName );

            /// Remove a breakpoint from a signal
            /// Slot string should be formatted as: [processor name]:[signal name]
            bool RemoveBreakpoint( const std::string& signal );
            /// Remove a breakpoint from a signal
            bool RemoveBreakpoint( const std::string& signalProcName, const std::string& signalName );

        protected:
            bool ParseSignalSlotName( const std::string& toParse, std::string& nameOfProc, std::string& nameOfSigSlot ) const;
            static const char fSigSlotNameSep = ':';

    };

    inline bool ProcessorToolbox::MakeConnection(const std::string& signal, const std::string& slot) 
    {
        return MakeConnection(signal, slot, std::numeric_limits< int >::min());
    }

    inline bool ProcessorToolbox::MakeConnection(const std::string& signalProcName, const std::string& signalName, const std::string& slotProcName, const std::string& slotName)
    {
        return MakeConnection(signalProcName, signalName, slotProcName, slotName, std::numeric_limits< int >::min()); 
    }

} /* namespace Nymph */
#endif /* NYMPH_PROCESSORTOOLBOX_HH_ */
