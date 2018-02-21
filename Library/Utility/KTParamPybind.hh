/*
 * KTPyNymph.cc
 *
 *  Created on: Feb 1, 2018
 *      Author: N. Oblath, L. Gladstone
 */

#include "param.hh"

#include "pybind11/pybind11.h"

namespace Nymph
{

    void ExportParamPybind( pybind11::module& mod )
    {
        pybind11::class_< scarab::param_value >( mod, "param_value" )
            .def( pybind11::init< bool >() )
            .def( pybind11::init< unsigned >() )
            .def( pybind11::init< int >() )
            .def( pybind11::init< double >() )
            .def( pybind11::init< const std::string& >() )

            .def( "as_bool", (bool (scarab::param_value::*)() const) &scarab::param_value::as_bool, "Get parameter value as a bool" )
            .def( "as_uint", (unsigned (scarab::param_value::*)() const) &scarab::param_value::as_uint, "Get parameter value as an unsigned integer" )
            .def( "as_int", (int (scarab::param_value::*)() const) &scarab::param_value::as_int, "Get parameter value as a signed integer" )
            .def( "as_double", (double (scarab::param_value::*)() const) &scarab::param_value::as_double, "Get parameter value as a float" )
            .def( "as_string", (std::string (scarab::param_value::*)() const) &scarab::param_value::as_string, "Get parameter value as a string" )

            .def( "set", (void (scarab::param_value::*)(bool)) &scarab::param_value::set, "Set a bool value" )
            .def( "set", (void (scarab::param_value::*)(unsigned)) &scarab::param_value::set, "Set an unsigned integer value" )
            .def( "set", (void (scarab::param_value::*)(int)) &scarab::param_value::set, "Set a signed integer value" )
            .def( "set", (void (scarab::param_value::*)(double)) &scarab::param_value::set, "Set an float value" )
            .def( "set", (void (scarab::param_value::*)(const std::string&)) &scarab::param_value::set, "Set an string value" )
            ;

        pybind11::class_< scarab::param_node >( mod, "param_node" )
            .def( pybind11::init<>() )
            
            .def( "add",(bool (scarab::param_node::*)(const scarab::param_value&) const) &scarab::param_value::get<bool>,
                    "Add a parameter value to a node" )

            // Get value of the parameter, when no default is set
            .def( "get_value", (bool (scarab::param_node::*)(const std::string&) const) &scarab::param_node::get_value<bool>,
                    "Get parameter node value as a bool" )
            .def( "get_value", (unsigned (scarab::param_node::*)(const std::string &) const) &scarab::param_node::get_value<uint>,
                    "Get parameter node value as an unsigned integer" )
            .def( "get_value", (int (scarab::param_node::*)(const std::string &) const) &scarab::param_node::get_value<int>,
                    "Get parameter node value as a signed integer" )
            .def( "get_value", (double (scarab::param_node::*)(const std::string &) const) &scarab::param_node::get_value<double>,
                    "Get parameter node value as a float" )
            .def( "get_value", (std::string (scarab::param_node::*)(const std::string &) const) &scarab::param_node::get_value,
                    "Get parameter node value as a string" )

            // Get value of the parameter, bringing along the default value
            .def( "get_value", (bool (scarab::param_node::*)(const std::string&, bool) const) &scarab::param_node::get_value<bool>,
                    "Get parameter node value as a bool" )
            .def( "get_value", (unsigned (scarab::param_node::*)(const std::string &, unsigned) const) &scarab::param_node::get_value<uint>,
                    "Get parameter node value as an unsigned integer" )
            .def( "get_value", (int (scarab::param_node::*)(const std::string &, int) const) &scarab::param_node::get_value<int>,
                    "Get parameter node value as a signed integer" )
            .def( "get_value", (double (scarab::param_node::*)(const std::string &, double) const) &scarab::param_node::get_value<double>,
                    "Get parameter node value as a float" )
            .def( "get_value", (std::string (scarab::param_node::*)(const std::string &, const std::string& ) const) &scarab::param_node::get_value,
                    "Get parameter node value as a string" )
            ;



    }
    
} /* namespace Nymph */
