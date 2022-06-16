/*
 * DataFramePybind.hh
 *
 *  Created on: Jun 16, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_DATA_DATAFRAMEPYBIND_HH_
#define PYTHON_BINDINGS_DATA_DATAFRAMEPYBIND_HH_

#include <pybind11/pybind11.h>
#include <memory>

#include "DataFrame.hh"


namespace py = pybind11;

namespace NymphPybind
{
    
    void ExportDataFrame( py::module_& nymphData)
    {
        py::class_< Nymph::DataFrame, std::shared_ptr<Nymph::DataFrame> >(nymphData, "_DataFrame")
                .def(py::init< >())
                
        ;
    }

}

#endif /* PYTHON_BINDINGS_DATA_DATAFRAMEPYBIND_HH_ */

