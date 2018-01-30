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

    void ExportTestPythonBasics( pybind11::module& mod )
    {
        mod.def( "add", &PyTest::add, "A function that adds two integers" );
    }

}
