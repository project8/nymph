/*
 * ProcessorPybind.hh
 *
 *  Created on: Feb 15, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_

#include <pybind11/pybind11.h>
#include <memory>

#include "NymphBindingHelpers.hh"
#include "Processor.hh"

#include "factory.hh"

namespace py = pybind11;

namespace NymphPybind
{
    //trampoline class
    class PyProcessor : public Nymph::Processor {
        public:
            /* Inherit the constructors */
            using Nymph::Processor::Processor;

            /* Trampoline (need one for each virtual function) */
            void Configure( const scarab::param_node& node ) override {
                PYBIND11_OVERRIDE_PURE_NAME(
                    void, /* Return type */
                    Processor,      /* Parent class */
                    "configure",  /*Name of function in python*/
                    Configure,          /* Name of function in C++ */
                    node/* Argument(s) */
                );
            }
    };
    
    void ExportProcessor( py::module_& nymphProcessor)
    {
        py::class_< Nymph::Processor, PyProcessor, std::shared_ptr<Nymph::Processor> >(nymphProcessor, "_Processor")
                .def(py::init<const std::string& >())
                .def("configure", &Nymph::Processor::Configure, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("connect_signal_to_slot", &Nymph::Processor::ConnectSignalToSlot, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("connect_a_slot", &Nymph::Processor::ConnectASlot, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("connect_a_signal", &Nymph::Processor::ConnectASignal, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("register_signal", &Nymph::Processor::RegisterSignal, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("register_slot", &Nymph::Processor::RegisterSlot, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("get_do_breakpoint", &Nymph::Processor::GetDoBreakpoint)
                .def("set_do_breakpoint", &Nymph::Processor::SetDoBreakpoint)
                .def_property("name", static_cast< const std::string& (Nymph::Processor::*)() const>(&Nymph::Processor::Name),
                                      [](Nymph::Processor& processor, const std::string& name){processor.Name() = name;} )
                .def_property_readonly("signals", static_cast< const std::map< std::string, Nymph::SignalBase* >& (Nymph::Processor::*)() const>(&Nymph::Processor::Signals))
                .def_property_readonly("slots", static_cast< const std::map< std::string, Nymph::SlotBase* >& (Nymph::Processor::*)() const>(&Nymph::Processor::Slots))
        ;
    }

}

#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_ */
