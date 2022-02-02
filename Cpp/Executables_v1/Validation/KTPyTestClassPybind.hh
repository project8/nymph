/*
 * KTPyTestClassPybind.cc
 *
 *  Created on: Jan 24, 2018
 *      Author: obla999
 */

#include "KTPyTestClass.hh"
#include "KTTestProcessor.hh"

#include "pybind11/pybind11.h"

namespace Nymph
{

    void ExportKTPyTestClass( pybind11::module& mod )
    {
        pybind11::class_< Nymph::KTPyTestClass >( mod, "KTPyTestClass" )
                .def( pybind11::init<>() )
                .def( "SayHello", &Nymph::KTPyTestClass::SayHello )
                .def( "GetValue", &Nymph::KTPyTestClass::GetValue )
                .def( "SetValue", &Nymph::KTPyTestClass::SetValue )
                ;
    };

} /* namespace Nymph */
