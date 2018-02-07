/*
 * NymphValidationPy.cc
 *
 *  Created on: Jan 25, 2018
 *      Author: obla999
 */

#include "KTPyTestClassPybind.hh"
#include "TestPythonBasics.hh"
#include "pybind11/pybind11.h"
#include <memory>

PYBIND11_MODULE( py_nymph_validation, mod )
{
    Nymph::ExportKTPyTestClass( mod );
    Nymph::ExportTestPythonBasics( mod );

    /*pybind11::class_<Nymph::KTWrapProcessor, Nymph::KTPyWrapProcessor> wrap_processor(mod, "WrapProcessor");
        wrap_processor
    		.def(pybind11::init<>())
    		.def("WrapFunction", &Nymph::KTWrapProcessor::WrapFunction)
        	.def("Configure", &Nymph::KTWrapProcessor::Configure);
*/
    //pybind11::class_< Nymph::KTProcessor, std::shared_ptr<Nymph::KTProcessor>>( mod, "KTProcessor" );

}


