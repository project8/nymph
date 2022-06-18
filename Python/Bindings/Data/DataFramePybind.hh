/*
 * DataFramePybind.hh
 *
 *  Created on: Jun 16, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_DATA_DATAPFRAMEPYBIND_HH_
#define PYTHON_BINDINGS_DATA_DATAPFRAMEPYBIND_HH_

#include "DataFrame.hh"

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

                .def( "has", []( const Nymph::DataFrame& frame, const std::string& typeName ){

                } )

                .def( "get" []( const Nymph::DataFrame& frame, const std::string& typeName ){
                    
                } )

                .def( "set" []( const Nymph::DataFrame& frame, std::unique_ptr<Nymph::Data>&& ptr ){
                    
                } )

                .def( "remove" []( const Nymph::DataFrame& frame, const std::string& typeName ){
                    
                } )

        ;

    }

}

#endif /* PYTHON_BINDINGS_DATA_DATAPFRAMEPYBIND_HH_ */

