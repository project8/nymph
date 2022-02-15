/*
 * NymphPybind.cc
 *
 *  Created on: Feb 14, 2022
 *      Author: F. Thomas
 */

#include <pybind11/pybind11.h>

#include "Processor/ProcessorPybind.hh"

namespace py = pybind11;


PYBIND11_MODULE(_nymph, nymphToplvl) {

	nymphToplvl.doc() = "Nymph package";

	auto nymphControl = nymphToplvl.def_submodule("control", "Control module");
	auto nymphData = nymphToplvl.def_submodule("data", "Data module");
	auto nymphImplementation = nymphToplvl.def_submodule("implementation", "Implementation module");
	auto nymphProcessor = nymphToplvl.def_submodule("processor", "Processor module");
	NymphPybind::ExportProcessor(nymphProcessor);
	auto nymphUtility = nymphToplvl.def_submodule("utility", "Utility module");
}
