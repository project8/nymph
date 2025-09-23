
"""
  testprocessorvisibility.py
 
   Created on: Sep 22, 2025
       Author: N.S. Oblath
"""
import unittest

import nymph_bindings #, _nymph_testing


class TestProcessorVisibility(unittest.TestCase):
    tb = nymph_bindings.processor.ProcessorToolbox()


    '''Testing whether the processor toolbox knows about certain processors'''
    def test_could_build(self):
        self.assertFalse( self.tb.could_build( 'does not exist' ) )  # Something that definitely shouldn't exist
        self.assertTrue( self.tb.could_build( 'hello-world-cpp' ) )  # hw-cpp exists in the C++ library, and therefore should be visible
        self.assertFalse( self.tb.could_build( 'hello-world-python' ) )  # Binding doesn't know about hw-python
    

if __name__ == '__main__':
    unittest.main()
