# TestPythonBasics.py
#  Created on: Jan 29, 2018
#      Author: N.S. Oblath
#
# Verifies that we can import Nymph python modules and call wrapped functions.
# Uses Nymph::PyTest::add(), which is defined in TestPythonBasics.cc

import py_nymph_validation

print( '1 + 2 =', py_nymph_validation.add(1, 2) )
