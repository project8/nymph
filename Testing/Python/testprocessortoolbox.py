
"""
  testprocessortoolbox.py
 
   Created on: Jun 14, 2022
       Author: N.S. Oblath
"""
import unittest

import _nymph, _nymph_testing


class TestDataMethods(unittest.TestCase):
    tb = _nymph.processor.ProcessorToolbox()

    procName1 = 'testproc-1'  # not a registered processor
    procName2 = 'testproc-2'  # not a registered processor
    procName3 = 'test-proc'   # a registered processor

    '''Testing Processor handling'''
    def test_add_remove_processors(self):
        self.assertFalse( self.tb.has_processor( self.procName1 ) )
        self.assertFalse( self.tb.has_processor( self.procName2 ) )
        self.assertFalse( self.tb.has_processor( self.procName3 ) )
        self.assertFalse( self.tb.could_build( self.procName1 ) )
        self.assertFalse( self.tb.could_build( self.procName2 ) )
        self.assertTrue( self.tb.could_build( self.procName3 ) )
    

if __name__ == '__main__':
    unittest.main()
