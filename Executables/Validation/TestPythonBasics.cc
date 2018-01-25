/*
 * TestPythonBasics.hh
 *
 *  Created on: Jan 24, 2018
 *      Author: obla999
 */

#include "pybind11/pybind11.h"


namespace Nymph
{
    namespace PyTest
    {
        int add( int i, int j )
        {
            return i + j;
        }
    }
}

PYBIND11_MODULE( nymph_validation, mod )
{
    mod.def( "add", &Nymph::PyTest::add, "A function that adds two integers" );
}
