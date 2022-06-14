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

#include "Processor.hh"
//#include "TestProcessorClasses.hh"
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
                PYBIND11_OVERRIDE_PURE(
                    void, /* Return type */
                    Processor,      /* Parent class */
                    Configure,          /* Name of function in C++ (must match Python name) */
                    node/* Argument(s) */
                );
            }
    };
    
    std::shared_ptr< Nymph::Processor > get_processor(const std::string& type, const std::string& name)
    {
        scarab::factory< Nymph::Processor, const std::string& >* factory = scarab::factory< Nymph::Processor, const std::string& >::get_instance();
        return std::shared_ptr< Nymph::Processor >(factory->create(type, name));
    }

    void ExportProcessor( py::module_& nymphProcessor)
    {
        py::class_< Nymph::Processor, PyProcessor, std::shared_ptr<Nymph::Processor> >(nymphProcessor, "_Processor")
                .def(py::init<const std::string& >())
                .def("configure", &Nymph::Processor::Configure)
                .def("connect_signal_to_slot", &Nymph::Processor::ConnectSignalToSlot)
                .def("connect_a_slot", &Nymph::Processor::ConnectASlot)
                .def("connect_a_signal", &Nymph::Processor::ConnectASignal)
                .def("register_signal", &Nymph::Processor::RegisterSignal)
                .def("register_slot", &Nymph::Processor::RegisterSlot)
                .def("get_do_breakpoint", &Nymph::Processor::GetDoBreakpoint)
                .def("set_do_breakpoint", &Nymph::Processor::SetDoBreakpoint);
                .def_property("name", static_cast< const std::string& (Nymph::Processor::*)() const>(&Nymph::Processor::Name),
                                      [](Nymph::Processor& processor, const std::string& name){processor.Name() = name;} )
                .def_property_readonly("signals", static_cast< const std::map< std::string, Nymph::SignalBase* >& (Nymph::Processor::*)() const>(&Nymph::Processor::Signals))
                .def_property_readonly("slots", static_cast< const std::map< std::string, Nymph::SlotBase* >& (Nymph::Processor::*)() const>(&Nymph::Processor::Slots));
        
        nymphProcessor.def("get_processor", &get_processor);
        
        //unfortunately this has to go into library code
        //for some reason the factory in the wrapped get_processor function above 
        //cannot find the test-proc when it is registered after the library was compiled
        //REGISTER_PROCESSOR(Nymph, TestProc, "test-proc");
    }

}

#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_ */
