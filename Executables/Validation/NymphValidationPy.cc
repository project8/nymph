/*
 * NymphValidationPy.cc
 *
 *  Created on: Jan 25, 2018
 *      Author: obla999
 */

#include "KTPyTestClassPybind.hh"
#include "TestPythonBasics.hh"

PYBIND11_MODULE( nymph_validation, mod )
{
    ExportKTPyTestClass( mod );
    ExportTestPythonBasics( mod );
}



