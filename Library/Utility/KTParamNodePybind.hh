/*
 * KTPyNymph.cc
 *
 *  Created on: Feb 1, 2018
 *      Author: N. Oblath, L. Gladstone
 */

#include "param_node.hh"
#include "pybind11/pybind11.h"

namespace Nymph
{

    void ExportParamNodePybind( pybind11::module& mod )
    {
        pybind11::class_< scarab::param_value >(mod, "ParamValue")
            .def(pybind11::init<bool>())
            .def("get",(bool (scarab::param_value::*)() const) &scarab::param_value::get<bool>,"Get parameter value")
            ;

        pybind11::class_< scarab::param_node >(mod, "ParamNode")
            .def(pybind11::init<>())
            
            .def("add",(bool (scarab::param_node::*)(const scarab::param_value&) const) &scarab::param_value::get<bool>,"Add a parameter value to a node")

            // Get value of the parameter, when no default is set
            .def("get_value", (bool (scarab::param_node::*)(const std::string&) const) &scarab::param_node::get_value<bool>, "Get parameter node value")
            .def("get_value", (uint (scarab::param_node::*)(const std::string &) const) 
                &scarab::param_node::get_value<uint>, "Get parameter node value")
            .def("get_value", (int (scarab::param_node::*)(const std::string &) const) 
                &scarab::param_node::get_value<int>, "Get parameter node value")
            .def("get_value", (double (scarab::param_node::*)(const std::string &) const) 
                &scarab::param_node::get_value<double>, "Get parameter node value")
            .def("get_value", (std::string (scarab::param_node::*)(const std::string &) const) 
                &scarab::param_node::get_value, "Get parameter node value")

            // Get value of the parameter, bringing along the default value
            .def("get_value", (bool (scarab::param_node::*)(const std::string&, bool) const) &scarab::param_node::get_value<bool>, "Get parameter node value")
            .def("get_value", (uint (scarab::param_node::*)(const std::string &, uint) const) 
                &scarab::param_node::get_value<uint>, "Get parameter node value")
            .def("get_value", (int (scarab::param_node::*)(const std::string &, int) const) 
                &scarab::param_node::get_value<int>, "Get parameter node value")
            .def("get_value", (double (scarab::param_node::*)(const std::string &, double) const) 
                &scarab::param_node::get_value<double>, "Get parameter node value")
            .def("get_value", (std::string (scarab::param_node::*)(const std::string &, const std::string & ) const) 
                &scarab::param_node::get_value, "Get parameter node value")
            ;



    }
    
} /* namespace Nymph */
