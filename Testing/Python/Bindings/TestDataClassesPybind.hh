/*
 * TestDataClassesPybind.hh
 *
 *  Created on: Jul 2, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_TESTING_TESTDATACLASSESPYBIND_HH_
#define PYTHON_BINDINGS_TESTING_TESTDATACLASSESPYBIND_HH_

#include "TestDataClasses.hh"

#include <pybind11/pybind11.h>
#include <memory>

namespace py = pybind11;

namespace NymphTestingPybind
{

    void ExportDataClasses( py::module_& nymphTesting)
    {
        py::class_< NymphTesting::TestData1 >( nymphTesting, "TestData1" )
                .def( py::init< >() )
        ;

        py::class_< NymphTesting::TestData2 >( nymphTesting, "TestData2" )
                .def( py::init< >() )
        ;
    }

}

#endif /* PYTHON_BINDINGS_TESTING_TESTDATACLASSESPYBIND_HH_ */

