/*
 * KTProcessorToolboxPybind.hh
 *
 *  Created on: Jan 29, 2018
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTPROCESSORTOOLBOXPYBIND_HH_
#define NYMPH_KTPROCESSORTOOLBOXPYBIND_HH_

#include "KTProcessorToolbox.hh"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace Nymph
{
    void ExportKTProcessorToolbox( pybind11::module& mod )
    {
        pybind11::class_< KTProcessorToolbox >( mod, "KTProcessorToolbox" )
                .def( pybind11::init< const std::string & >() )

                // members related to configuration
                .def( "ConfigureProcessors", (bool (KTProcessorToolbox::*)(const std::string&)) &KTProcessorToolbox::ConfigureProcessors,
                        "Configure processors from a json dictionary. Top-level keys are processor names, values are dictionaries with their configurations" )

                // processor access
                // GetProcessor skipped because raw pointers are not supported by pybind11
                .def( "AddProcessor", (bool (KTProcessorToolbox::*)(const std::string&, const std::string&)) &KTProcessorToolbox::AddProcessor,
                        "Create a processor by name and add it to the toolbox" )
				.def("AddProcessor", (bool (KTProcessorToolbox::*)(const std::string&, std::shared_ptr< KTProcessor > proc)) &KTProcessorToolbox::AddProcessor,
						                       "Create a processor and add it to the toolbox" )
                .def( "RemoveProcessor", &KTProcessorToolbox::RemoveProcessor,
                        "Remove a processor from the toolbox" )
                // skipping ReleaseProcessor
                .def( "ClearProcessor", &KTProcessorToolbox::ClearProcessors,
                        "Remove all processors from the toolbox" )

                // processor connections
                .def( "MakeConnection", (bool (KTProcessorToolbox::*)(const std::string&, const std::string&, int)) &KTProcessorToolbox::MakeConnection,
                        "Make a connection between the signal of one processor and the slot of another processor; the signal and slot strings should be formatted as: [processor name]:[signal/slot name]" )
                .def( "MakeConnection", (bool (KTProcessorToolbox::*)(const std::string&, const std::string&)) &KTProcessorToolbox::MakeConnection,
                        "Make a connection between the signal of one processor and the slot of another processor; the signal and slot strings should be formatted as: [processor name]:[signal/slot name]" )
                .def( "MakeConnection", (bool (KTProcessorToolbox::*)(const std::string&, const std::string&, const std::string&, const std::string&, int)) &KTProcessorToolbox::MakeConnection,
                        "Make a connection between the signal of one processor and the slot of another processor" )
                .def( "MakeConnection", (bool (KTProcessorToolbox::*)(const std::string&, const std::string&, const std::string&, const std::string&)) &KTProcessorToolbox::MakeConnection,
                        "Make a connection between the signal of one processor and the slot of another processor" )

                // breakpoints
                .def( "SetBreakpoint", (bool (KTProcessorToolbox::*)(const std::string&)) &KTProcessorToolbox::SetBreakpoint,
                        "Set a breakpoint at a specific slot; slot string should be formatted as: [processor name]:[slot name]" )
                .def( "SetBreakpoint", (bool (KTProcessorToolbox::*)(const std::string&, const std::string&)) &KTProcessorToolbox::SetBreakpoint,
                        "Set a breakpoint at a specific slot" )

                .def( "RemoveBreakpoint", (bool (KTProcessorToolbox::*)(const std::string&)) &KTProcessorToolbox::RemoveBreakpoint,
                        "Remove a breakpoint; slot string should be formatted as: [processor name]:[slot name]" )
                .def( "RemoveBreakpoint", (bool (KTProcessorToolbox::*)(const std::string&, const std::string&)) &KTProcessorToolbox::RemoveBreakpoint,
                        "Remove a breakpoint" )

                // run queue
                .def( "PushBackToRunQueue", (bool (KTProcessorToolbox::*)(const std::string&)) &KTProcessorToolbox::PushBackToRunQueue,
                        "Push one or more processors to the back of the run queue using a comma-separated string" )
                .def( "PushBackToRunQueue", (bool (KTProcessorToolbox::*)(std::vector< std::string >)) &KTProcessorToolbox::PushBackToRunQueue,
                        "Push one or more processors to the back of the run queue using a vector of strings" )

                .def( "PopBackOfRunQueue", &KTProcessorToolbox::PopBackOfRunQueue,
                        "Remove the last item in the run queue, whether it's a single processor or a group" )
                .def( "ClearRunQueue", &KTProcessorToolbox::ClearRunQueue,
                        "Remove all items from the run queue" )

                // run methods
                .def( "Run", &KTProcessorToolbox::Run, "Start the processors listed in the run queue (blocking)" )
                .def( "AsyncRun", &KTProcessorToolbox::AsyncRun, "Non-blocking call to Run()" )
                .def( "WaitForBreak", &KTProcessorToolbox::WaitForBreak, "Blocking call that waits until a breakpoint is reached; you must manually Continue() to proceed" )
                .def( "WaitForEndOfRun", &KTProcessorToolbox::WaitForEndOfRun, "Blocking call to Continue() execution until the end of the run is reached; you must manually Continue() to proceed" )
                .def( "Continue", &KTProcessorToolbox::Continue, "Resume execution (non-blocking)" )
                .def( "CancelThreads", &KTProcessorToolbox::CancelThreads, "Kill any running thread at the next breakpoint check" )
                .def( "JoinRunThread", &KTProcessorToolbox::JoinRunThread, "Blocking call to wait until Run() is complete; this should only be done by the owner of the thread (typically whoever called Run())" )

                .def( "GetData", &KTProcessorToolbox::GetData, "Retrieve the data handle from a slot by thread name (i.e. the name of the primary processor for that thread)" );


                /*
                // these are implemented in the boost::python version but not yet here

                PROPERTYMEMBER(KTProcessorToolbox, RunSingleThreaded)

                // Processor access
                .def("GetProcessor", GetProcessor_wrap, return_value_policy<reference_existing_object>(), "Get a pointer to a processor in the toolbox")
                .def("AddProcessor", AddProcessor_Ref, "add a processor to the toolbox, toolbox takes ownership")
                // TODO: Not 100% certain that the reference count for this processor is now correct, given the return_value_policy
                .def("ReleaseProcessor", &KTProcessorToolbox::ReleaseProcessor, return_value_policy<reference_existing_object>(), "Remove a processor from the toolbox and return it to the user, ownership is passed")
                */
                ;

    }

}

#endif /* NYMPH_KTPROCESSORTOOLBOXPYBIND_HH_ */
