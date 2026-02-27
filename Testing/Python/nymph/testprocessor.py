
"""
   testprocessor.py
 
   Created on: Sep 22, 2025
       Author: N.S. Oblath
"""
import unittest

import nymph #, _nymph_testing
import scarab


class TestProcessor(unittest.TestCase):

    '''Testing whether the processor toolbox knows about certain processors'''
    def test_basics(self):
        proc = nymph.processor.Processor( "test" )
        with self.assertRaises(NotImplementedError):
            proc.configure(scarab.to_param({}))

    def test_helloworld(self):
        proc = nymph.processor.HelloWorldPython( "hw" )
        self.assertIsInstance( proc, nymph.processor.Processor )


if __name__ == '__main__':
    unittest.main()
