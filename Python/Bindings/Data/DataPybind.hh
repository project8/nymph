/*
 * DataPybind.hh
 *
 *  Created on: Jun 13, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_DATA_DATAPYBIND_HH_
#define PYTHON_BINDINGS_DATA_DATAPYBIND_HH_

#include <pybind11/pybind11.h>
#include <memory>

#include "Data.hh"

namespace py = pybind11;

namespace NymphPybind
{
    //trampoline class
    class PyData : public Nymph::Data {
        public:
            /* Inherit the constructors */
            using Nymph::Data::Data;

    };

    void ExportData( py::module_& nymphData )
    {
        py::class_< Nymph::Data, PyData >( nymphData, "_Data" )
                .def( py::init< >() )
                .def( "get_name", &Nymph::Data::GetName )
        ;
    }

}

#endif /* PYTHON_BINDINGS_DATA_DATAPYBIND_HH_ */

