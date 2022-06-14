/*
 * PyProcCreatorPybind.hh
 *
 *  Created on: Jun 13, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PYPROCCREATORPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_PYPROCCREATORPYBIND_HH_

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace NymphPybind
{
    void ExportPyProcCreator( py::module_& nymphProcessor)
    {
        py::class_< PyProcCreator >(nymphProcessor, "_PyProcCreator")
                .def(py::init<const std::string& >())
                .def("create_python_processor", &PyProcCreator::CreatePythonProcessor);
        
        nymphProcessor.def("register_py_processor", &RegisterPyProcessor);
    }
}

#endif /* PYTHON_BINDINGS_PROCESSOR_PYPROCCREATORPYBIND_HH_ */
