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

void export_ProcessorToolbox()
{
    using namespace Nymph;
    using namespace boost::python;
    class_<KTProcessorToolbox, boost::noncopyable>("KTProcessorToolbox", init<std::string>())
        .def("Run", &KTProcessorToolbox::Run, "Call Run() on all processors in the run queue")

        // Not adding Configure methods as they are for conf files, not interactive sessions

        //.def("GetProcessor", &KTProcessorToolbox::GetProcessor, "Get a pointer to a processor in the toolbox")
        //TODO: what does this ownership statement mean in the context of boost::python? Also doesn't build
        .def("AddProcessor", &KTProcessorToolbox::AddProcessor, "add a processor to the toolbox, toolbox takes ownership")
        .def("RemoveProcessor", &KTProcessorToolbox::RemoveProcessor, "remove a processor from the toolbox")
        //TODO this next one doesn't build
        //.def("ReleaseProcessor", &KTProcessorToolbox::ReleaseProcessor, "Remove a processor from the toolbox and return it to the user, ownership is passed") // what does this passed ownership mean in boost::python?
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
