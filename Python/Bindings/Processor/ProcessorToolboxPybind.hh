/*
 * ProcessorToolboxPybind.hh
 *
 *  Created on: Feb 15, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PROCESSORTOOLBOXPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_PROCESSORTOOLBOXPYBIND_HH_

#include <pybind11/pybind11.h>
#include <memory>

#include "Processor.hh"
//#include "TestProcessorClasses.hh"
#include "factory.hh"

namespace py = pybind11;

namespace NymphPybind
{
    void ExportProcessorToolbox( py::module_& nymphProcessor)
    {
        py::class_< Nymph::ProcessorToolbox >(nymphProcessor, "ProcessorToolbox")
                .def(py::init<const std::string& >())
                .def("configure", &Nymph::Processor::Configure)
                
                .def("configure_processors", &Nymph::Processor::ConnectSignalToSlot)
                .def("get_processor", &Nymph::Processor::ConnectASlot) //
                .def("add_processor", &Nymph::Processor::ConnectASignal) //
                .def("remove_processor", &Nymph::Processor::RegisterSignal)
                .def("release_processor", &Nymph::Processor::RegisterSlot) //
                .def("clear_processors", &Nymph::Processor::GetDoBreakpoint)

                .def("configure_connections", &Nymph::Processor::SetDoBreakpoint) //
                .def("make_connection", &Nymph::Processor::SetDoBreakpoint) //

                .def("set_breakpoint", &Nymph::Processor::SetDoBreakpoint) //
                .def("remove_breakpoint", &Nymph::Processor::SetDoBreakpoint) //
        ;
        
    }

}

#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORTOOLBOXPYBIND_HH_ */
