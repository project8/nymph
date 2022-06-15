/*
 * ProcessorToolboxPybind.hh
 *
 *  Created on: Jun 14, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PROCESSORTOOLBOXPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_PROCESSORTOOLBOXPYBIND_HH_

#include "ProcessorToolbox.hh"

#include "Processor.hh"

#include <pybind11/pybind11.h>
#include <pybind11/cast.h>

#include <memory>

namespace py = pybind11;
using namespace py::literals;

namespace NymphPybind
{
    void ExportProcessorToolbox( py::module_& nymphProcessor )
    {
        py::class_< Nymph::ProcessorToolbox >( nymphProcessor, "ProcessorToolbox" )
                .def( py::init<const std::string& >(), "name"_a="processor-toolbox" )
                .def( "configure", &Nymph::ProcessorToolbox::Configure )
                
                .def( "configure_processors", &Nymph::ProcessorToolbox::ConfigureProcessors )
                .def( "get_processor", py::overload_cast< const std::string& >(&Nymph::ProcessorToolbox::GetProcessor) )
                .def( "get_processor_const", py::overload_cast< const std::string& >(&Nymph::ProcessorToolbox::GetProcessor, py::const_ ) )
                .def( "add_processor", py::overload_cast< const std::string&, std::shared_ptr<Nymph::Processor> >(&Nymph::ProcessorToolbox::AddProcessor) )
                .def( "add_processor", py::overload_cast< const std::string&, const std::string& >(&Nymph::ProcessorToolbox::AddProcessor) )
                .def( "has_processor", &Nymph::ProcessorToolbox::HasProcessor )
                .def( "could_build", &Nymph::ProcessorToolbox::CouldBuild )
                .def( "remove_processor", &Nymph::ProcessorToolbox::RemoveProcessor )
                .def( "release_processor", &Nymph::ProcessorToolbox::ReleaseProcessor )
                .def( "clear_processors", &Nymph::ProcessorToolbox::ClearProcessors )

                .def( "configure_connections", &Nymph::ProcessorToolbox::ConfigureConnections )
                .def( "make_connection", py::overload_cast< const std::string&, const std::string&, int >(&Nymph::ProcessorToolbox::MakeConnection) )
                .def( "make_connection", py::overload_cast< const std::string&, const std::string& >(&Nymph::ProcessorToolbox::MakeConnection) )
                .def( "make_connection", py::overload_cast< const std::string&, const std::string&, const std::string&, const std::string&, int >(&Nymph::ProcessorToolbox::MakeConnection) )
                .def( "make_connection", py::overload_cast< const std::string&, const std::string&, const std::string&, const std::string& >(&Nymph::ProcessorToolbox::MakeConnection) )

                .def( "set_breakpoint",  py::overload_cast< const std::string& >(&Nymph::ProcessorToolbox::SetBreakpoint) ) //
                .def( "set_breakpoint",  py::overload_cast< const std::string&, const std::string& >(&Nymph::ProcessorToolbox::SetBreakpoint) ) //
                .def( "remove_breakpoint", py::overload_cast< const std::string& >(&Nymph::ProcessorToolbox::RemoveBreakpoint) ) //
                .def( "remove_breakpoint", py::overload_cast< const std::string&, const std::string& >(&Nymph::ProcessorToolbox::RemoveBreakpoint) ) //
        ;
    }

}

#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORTOOLBOXPYBIND_HH_ */
