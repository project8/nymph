/*
 * DataFramePybind.hh
 *
 *  Created on: Jun 16, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_DATA_DATAPFRAMEPYBIND_HH_
#define PYTHON_BINDINGS_DATA_DATAPFRAMEPYBIND_HH_

#include "DataFrame.hh"

#include "DataPybind.hh"

#include <pybind11/pybind11.h>
#include <memory>

namespace py = pybind11;

namespace NymphPybind
{

    void ExportDataFrame( py::module_& nymphData)
    {
        py::class_< Nymph::DataFrame, std::shared_ptr<Nymph::DataFrame> >( nymphData, "DataFrame" )
                .def( py::init< >() )
                .def( "empty", &Nymph::DataFrame::Empty )
                .def( "has", static_cast< bool (Nymph::DataFrame::*)(const std::string&) const >( &Nymph::DataFrame::Has ) )
                .def( "get", 
                    static_cast< Nymph::Data& (Nymph::DataFrame::*)(const std::string&) >( &Nymph::DataFrame::Get ), 
                    py::return_value_policy::reference )
                // NOTE: per pybind11 docs, python will not give up ownership of an object to the container, so we cannot bind the Set() function
                //       see, e.g. https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html, where this rule is mentioned
                //.def( "set", 
                //    static_cast< void (Nymph::DataFrame::*)(const std::string &, std::unique_ptr<Nymph::Data>&&) >( &Nymph::DataFrame::Set ),
                //    py::keep_alive<1, 3>() )
                .def( "remove", static_cast< void (Nymph::DataFrame::*)(const std::string&) >( &Nymph::DataFrame::Remove ) )
        ;

    }

}

#endif /* PYTHON_BINDINGS_DATA_DATAPFRAMEPYBIND_HH_ */

