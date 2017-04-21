/**
 @file KTProcessorToolboxPy.hh
 @brief Contains KTProcessorToolbox python bindings
 @details Manages processors requested by the user at run time.
 @author: B. H. LaRoque 
 @date: April 20, 2017
 */

#ifndef KTPROCESSORTOOLBOXPY_HH_
#define KTPROCESSORTOOLBOXPY_HH_

#include "KTProcessorToolbox.hh"

//class KTProcessorToolboxWrapper : public KTProcessorToolbox
//{
//    public:
    bool (Nymph::KTProcessorToolbox::*PushBackToRunQueue_string)(const std::string& name) = &Nymph::KTProcessorToolbox::PushBackToRunQueue;
        //bool (KTProcessorToolboxWrapper::*PushBackToRunQueue_init_list)(std::initializer_list< std::string >) = &KTProcessorToolbox::PushBackToRunQueue;
        //bool (KTProcessorToolboxWrapper::*PushBackToRunQueue_vector)(std::vector< std::string >) = &KTProcessorToolbox::PushBackToRunQueue;
//}

void export_ProcessorToolbox()
{
    using namespace Nymph;
    using namespace boost::python;
    class_<KTProcessorToolbox, boost::noncopyable>("KTProcessorToolbox", init<std::string>())
        .def("Run", &KTProcessorToolbox::Run, "Call Run() on all processors in the run queue")

        // Push new processors to back of run queue
        .def("PushBackToRunQueue", PushBackToRunQueue_string, "Push a single processor to the back of the run queue")

        // Remove items from run queue
        .def("PopBackOfRunQueue", &KTProcessorToolbox::PopBackOfRunQueue, "Remove the last item in the run queue, whether it's a single processor or a group of processors")
        .def("ClearRunQueue", &KTProcessorToolbox::ClearRunQueue, "Clear the run queue")
        ;
}
/*
        public:
            /// Configure the toolbox: create the processors; connnect signals and slots; and setup the run queue.
            bool Configure(const scarab::param_node* node);

            /// Configure processors (only those specified in the toolbox)
            bool ConfigureProcessors(const scarab::param_node* node);


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


        public:
            /// Make a connection between the signal from one processor and the slot from another processor
            /// Both processors should already have been added to the Toolbox
            /// Signal and slot strings should be formatted as: [processor name]:[signal/slot name]
            bool MakeConnection(const std::string& signal, const std::string& slot, int order = std::numeric_limits< int >::min());

            /// Make a connection between the signal from one processor and the slot from another processor
            /// Both processors should already have been added to the Toolbox
            bool MakeConnection(const std::string& signalProcName, const std::string& signalName, const std::string& slotProcName, const std::string& slotName, int order = std::numeric_limits< int >::min());


        public:
            /// Push a single processor to the back of the run queue
//            bool PushBackToRunQueue(const std::string& name);

            /// Push a set of processors to be run in parallel to the back of the run queue
            bool PushBackToRunQueue(std::initializer_list< std::string > names);
            /// Push a set of processors to be run in parallel to the back of the run queue
            bool PushBackToRunQueue(std::vector< std::string > names);


*/
#endif /* KTPROCESSORTOOLBOXPY_HH_ */
