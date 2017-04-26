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

// Make connection overloads
   /* default values here? */
bool (Nymph::KTProcessorToolbox::*MakeConnection_3args)(const std::string&, const std::string&, int order) = &Nymph::KTProcessorToolbox::MakeConnection;
bool (Nymph::KTProcessorToolbox::*MakeConnection_4args)(const std::string&, const std::string&, const std::string&, const std::string&, int order) = &Nymph::KTProcessorToolbox::MakeConnection;

// Run queue pushback overloads
bool (Nymph::KTProcessorToolbox::*PushBackToRunQueue_string)(const std::string& name) = &Nymph::KTProcessorToolbox::PushBackToRunQueue;
bool (Nymph::KTProcessorToolbox::*PushBackToRunQueue_init_list)(std::initializer_list< std::string >) = &Nymph::KTProcessorToolbox::PushBackToRunQueue;
bool (Nymph::KTProcessorToolbox::*PushBackToRunQueue_vector)(std::vector< std::string >) = &Nymph::KTProcessorToolbox::PushBackToRunQueue;

// Add Processor overloads
bool (Nymph::KTProcessorToolbox::*AddProcessor_Ref)(const std::string&, Nymph::KTProcessor*) = &Nymph::KTProcessorToolbox::AddProcessor;
bool (Nymph::KTProcessorToolbox::*AddProcessor_TypeStr)(const std::string&, const std::string&) = &Nymph::KTProcessorToolbox::AddProcessor;

void export_ProcessorToolbox()
{
    using namespace Nymph;
    using namespace boost::python;
    class_<KTProcessorToolbox, boost::noncopyable>("KTProcessorToolbox", init<std::string>())
        .def("Run", &KTProcessorToolbox::Run, "Call Run() on all processors in the run queue")

        // Not adding Configure methods as they are for conf files, not interactive sessions

        //TODO: not sure why this won't build, maybe because there is a const and non-const signature?
        //.def("GetProcessor", &KTProcessorToolbox::GetProcessor, return_value_policy<reference_existing_object>(), "Get a pointer to a processor in the toolbox")
        .def("AddProcessor", AddProcessor_Ref, "add a processor to the toolbox, toolbox takes ownership")
        .def("AddProcessor", AddProcessor_TypeStr, "add a processor to the toolbox, toolbox takes ownership")
        .def("RemoveProcessor", &KTProcessorToolbox::RemoveProcessor, "remove a processor from the toolbox")

        //TODO: Not 100% certain that the reference count for this processor is now correct, given the return_value_policy
        .def("ReleaseProcessor", &KTProcessorToolbox::ReleaseProcessor, return_value_policy<reference_existing_object>(), "Remove a processor from the toolbox and return it to the user, ownership is passed")

        .def("ClearProcessors", &KTProcessorToolbox::ClearProcessors, "Remove all processors and clear run queue")

        // make signal-slot connection
        .def("MakeConnection", MakeConnection_3args, "Make a signal-slot connection")
        .def("MakeConnection", MakeConnection_4args)

        // Push new processor(s) to back of run queue
        .def("PushBackToRunQueue", PushBackToRunQueue_string, "Push processor(s) to the back of the run queue")
        .def("PushBackToRunQueue", PushBackToRunQueue_init_list)
        .def("PushBackToRunQueue", PushBackToRunQueue_vector)

        // Remove items from run queue
        .def("PopBackOfRunQueue", &KTProcessorToolbox::PopBackOfRunQueue, "Remove the last item in the run queue, whether it's a single processor or a group of processors")
        .def("ClearRunQueue", &KTProcessorToolbox::ClearRunQueue, "Clear the run queue")
        ;
}

#endif /* KTPROCESSORTOOLBOXPY_HH_ */
