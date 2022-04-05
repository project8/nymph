/*
 * ProcessorPybind.hh
 *
 *  Created on: Feb 15, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_

#include <pybind11/pybind11.h>

#include "Processor.hh"

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

	void ExportProcessor( py::module_& nymphProcessor)
	{
	    py::class_< Nymph::Processor, PyProcessor>(nymphProcessor, "_Processor")
	    		.def(py::init<const std::string& >())
				.def("configure", &Nymph::Processor::Configure)
				.def("connect_signal_to_slot", &Nymph::Processor::ConnectSignalToSlot)
				.def("connect_a_slot", &Nymph::Processor::ConnectASlot)
				.def("connect_a_signal", &Nymph::Processor::ConnectASignal)
				.def("register_signal", &Nymph::Processor::RegisterSignal)
				.def("register_slot", &Nymph::Processor::RegisterSlot)
				.def("get_do_breakpoint", &Nymph::Processor::GetDoBreakpoint)
				.def("set_do_breakpoint", &Nymph::Processor::SetDoBreakpoint);
				//.def_property_readonly("name", &Nymph::Processor::Name);
	}

}

#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_ */
