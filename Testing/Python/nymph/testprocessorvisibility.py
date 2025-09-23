
"""
  testprocessorvisibility.py
 
   Created on: Sep 22, 2025
       Author: N.S. Oblath
"""
import unittest

import nymph #, _nymph_testing


class TestProcessorVisibility(unittest.TestCase):
    tb = nymph.processor.ProcessorToolbox()

    '''Testing whether the processor toolbox knows about certain processors'''
    def test_could_build(self):
        self.assertFalse( self.tb.could_build( 'does not exist' ) )
        self.assertTrue( self.tb.could_build( 'hello-world-cpp' ) )
        self.assesrtTrue( self.tb.could_build( 'hello-world-python' ) )
    

if __name__ == '__main__':
    unittest.main()
