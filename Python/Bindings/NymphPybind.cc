/*
 * NymphPybind.cc
 *
 *  Created on: Feb 14, 2022
 *      Author: F. Thomas
 */

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(_nymph, m) {
	m.doc() = "nymph module";



}
