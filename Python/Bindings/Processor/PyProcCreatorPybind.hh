/*
 * PyProcCreatorPybind.hh
 *
 *  Created on: Jun 13, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PYPROCCREATORPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_PYPROCCREATORPYBIND_HH_

#include "ProcessorRegistrar.hh"

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace NymphPybind
{
    void ExportPyProcCreator( py::module_& nymphProcessor )
    {
        
        py::class_< Nymph::PyProcRegistrar >(nymphProcessor, "_PyProcRegistrar")
                .def(py::init< const std::string&, const std::string&, const std::string& >())
                .def("create_python_processor", &Nymph::PyProcRegistrar::CreatePyProc);
        
        nymphProcessor.def("register_py_processor", &Nymph::RegisterPyProcessor);
        nymphProcessor.def("create_processor", &Nymph::CreatePyProcessor);
    }
}

#endif /* PYTHON_BINDINGS_PROCESSOR_PYPROCCREATORPYBIND_HH_ */
