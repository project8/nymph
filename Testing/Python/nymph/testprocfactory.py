"""
   testprocfactory.py

   Testing of the Processor factory functions
 
   Created on: Sep 29, 2025
       Author: N.S. Oblath
"""
import unittest

import nymph
import scarab


class TestNoProc(unittest.TestCase):
    '''Testing the situation where the processor type doesn't exist'''
    def test_noproc(self):
        with self.assertRaises(RuntimeError) as cm:
            nymph.processor.create( "blah", "blah-blah" )
        the_exception = cm.exception
        self.assertEqual(str(the_exception), "Did not find processor with type <blah>")

class TestCppProc(unittest.TestCase):
    '''Testing the creation of a C++ processor, which won't work'''
    def test_cppproc(self):
        with self.assertRaises(RuntimeError) as cm:
            nymph.processor.create( "hello-world-cpp", "hw" )
        the_exception = cm.exception
        self.assertEqual(str(the_exception), "Registrar did not cast correctly for <hello-world-cpp>")

class TestPyProc(unittest.TestCase):
    '''Testing the creation of a Python processor, which should work'''
    def test_pyproc(self):
        proc = nymph.processor.create( "hello-world-py", "hw" )


if __name__ == '__main__':
    unittest.main()
