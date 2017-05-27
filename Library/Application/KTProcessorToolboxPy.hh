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
#include "KTPythonMacros.hh"

// Make connection overloads
   /* need two macros to cover all signatures*/
bool (Nymph::KTProcessorToolbox::*MakeConnection_2names)(const std::string&, const std::string&, int order) = &Nymph::KTProcessorToolbox::MakeConnection;
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MakeConnection_2names_overloads, Nymph::KTProcessorToolbox::MakeConnection, 2, 3)
bool (Nymph::KTProcessorToolbox::*MakeConnection_4names)(const std::string&, const std::string&, const std::string&, const std::string&, int order) = &Nymph::KTProcessorToolbox::MakeConnection;
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MakeConnection_4names_overloads, Nymph::KTProcessorToolbox::MakeConnection, 4, 5)

// Set and Remove breakpoint overloads
bool (Nymph::KTProcessorToolbox::*SetBreakpoint_1arg)(const std::string&) = &Nymph::KTProcessorToolbox::SetBreakpoint;
bool (Nymph::KTProcessorToolbox::*SetBreakpoint_2arg)(const std::string&, const std::string&) = &Nymph::KTProcessorToolbox::SetBreakpoint;
bool (Nymph::KTProcessorToolbox::*RemoveBreakpoint_1arg)(const std::string&) = &Nymph::KTProcessorToolbox::RemoveBreakpoint;
bool (Nymph::KTProcessorToolbox::*RemoveBreakpoint_2arg)(const std::string&, const std::string&) = &Nymph::KTProcessorToolbox::RemoveBreakpoint;
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetBreakpoint_overloads, Nymph::KTProcessorToolbox::SetBreakpoint, 1, 2)
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(RemoveBreakpoint_overloads, Nymph::KTProcessorToolbox::SetBreakpoint, 1, 2)

// Run queue pushback overloads
bool (Nymph::KTProcessorToolbox::*PushBackToRunQueue_string)(const std::string& name) = &Nymph::KTProcessorToolbox::PushBackToRunQueue;
bool (Nymph::KTProcessorToolbox::*PushBackToRunQueue_init_list)(std::initializer_list< std::string >) = &Nymph::KTProcessorToolbox::PushBackToRunQueue;
bool (Nymph::KTProcessorToolbox::*PushBackToRunQueue_vector)(std::vector< std::string >) = &Nymph::KTProcessorToolbox::PushBackToRunQueue;

// Add Processor overloads
bool (Nymph::KTProcessorToolbox::*AddProcessor_Ref)(const std::string&, Nymph::KTProcessor*) = &Nymph::KTProcessorToolbox::AddProcessor;
bool (Nymph::KTProcessorToolbox::*AddProcessor_TypeStr)(const std::string&, const std::string&) = &Nymph::KTProcessorToolbox::AddProcessor;

// Get Processor overloads
Nymph::KTProcessor* (Nymph::KTProcessorToolbox::*GetProcessor_wrap)(const std::string&) = &Nymph::KTProcessorToolbox::GetProcessor;

// Configure Processor overloads
bool (Nymph::KTProcessorToolbox::*ConfigureProcessors_JsonStr)(const std::string&) = &Nymph::KTProcessorToolbox::ConfigureProcessors;

void export_ProcessorToolbox()
{
    using namespace Nymph;
    using namespace boost::python;
    class_<KTProcessorToolbox, boost::noncopyable>("KTProcessorToolbox", init<std::string>())

        // members related to configuration
        .def("ConfigureProcessors", ConfigureProcessors_JsonStr, "Configure processors from a json dictionary. Top-level keys are processor names, values are dictionaries with their configurations")
        PROPERTYMEMBER(KTProcessorToolbox, RunSingleThreaded)


        // Processor access
        .def("GetProcessor", GetProcessor_wrap, return_value_policy<reference_existing_object>(), "Get a pointer to a processor in the toolbox")
        .def("AddProcessor", AddProcessor_Ref, "add a processor to the toolbox, toolbox takes ownership")
        .def("AddProcessor", AddProcessor_TypeStr, "add a processor to the toolbox, toolbox takes ownership")
        .def("RemoveProcessor", &KTProcessorToolbox::RemoveProcessor, "remove a processor from the toolbox")
        /* TODO: Not 100% certain that the reference count for this processor is now correct, given the return_value_policy */
        .def("ReleaseProcessor", &KTProcessorToolbox::ReleaseProcessor, return_value_policy<reference_existing_object>(), "Remove a processor from the toolbox and return it to the user, ownership is passed")
        .def("ClearProcessors", &KTProcessorToolbox::ClearProcessors, "Remove all processors and clear run queue")

        // Processor connections
        .def("MakeConnection", MakeConnection_2names, MakeConnection_2names_overloads())
        .def("MakeConnection", MakeConnection_4names, MakeConnection_4names_overloads())
        /* TODO use macro for arg counts?*/
        .def("SetBreakpoint", SetBreakpoint_1arg, "add breakpoint to 'procName:slotName'")
        .def("SetBreakpoint", SetBreakpoint_2arg, "add breakpoint to 'procName, slotName'")
        .def("RemoveBreakpoint", RemoveBreakpoint_1arg, "add breakpoint to 'procName:slotName'")
        .def("RemoveBreakpoint", RemoveBreakpoint_2arg, "add breakpoint to 'procName, slotName'")

        // Run Queue
        // Push new processor(s) to back of run queue
        .def("PushBackToRunQueue", PushBackToRunQueue_string, "Push processor(s) to the back of the run queue")
        .def("PushBackToRunQueue", PushBackToRunQueue_init_list)
        .def("PushBackToRunQueue", PushBackToRunQueue_vector)

        // Remove items from run queue
        .def("PopBackOfRunQueue", &KTProcessorToolbox::PopBackOfRunQueue, "Remove the last item in the run queue, whether it's a single processor or a group of processors")
        .def("ClearRunQueue", &KTProcessorToolbox::ClearRunQueue, "Clear the run queue")

        // Run methods
        .def("Run", &KTProcessorToolbox::Run, "Call Run() on all processors in the run queue")
        .def("AsyncRun", &KTProcessorToolbox::AsyncRun, "Non-blocking call to Run()")
        .def("WaitForContinue", &KTProcessorToolbox::WaitForContinue)
        .def("WaitForBreak", &KTProcessorToolbox::WaitForBreak, "blocking call that waits until a breakpoint is reached, you must manually Continue()")
        .def("WaitForEndOfRun", &KTProcessorToolbox::WaitForEndOfRun, "blocking call to Continue() execution until end of run is reached, you must manually Continue()")
        .def("Continue", &KTProcessorToolbox::Continue, "non-blocking call to resume execution")
        .def("CancelThreads", &KTProcessorToolbox::CancelThreads, "kill any running thread at the next breakpoint check")
        .def("JoinRunThread", &KTProcessorToolbox::JoinRunThread)

        .def("GetData", &KTProcessorToolbox::GetData)
        ;
}

#endif /* KTPROCESSORTOOLBOXPY_HH_ */
