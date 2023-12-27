/*
 * NymphTestingPybind.cc
 *
 *  Created on: Jun 14, 2022
 *      Author: N.S. Oblath
 */

#include <pybind11/pybind11.h>

#include "TestDataClassesPybind.hh"

namespace py = pybind11;


PYBIND11_MODULE(_nymph_testing, nymphTestingPackage) 
{

    nymphTestingPackage.doc() = "Nymph Testing package";

    auto nymphTesting = nymphTestingPackage.def_submodule("nymphTesting", "Nymph Testing module");
    NymphTestingPybind::ExportDataClasses(nymphTestingPackage);

}
