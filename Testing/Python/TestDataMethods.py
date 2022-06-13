
"""
  TestDataMethods.py
 
   Created on: Jun 13, 2022
       Author: P. T. Surukuchi
"""
import unittest

import _nymph

class TestData(_nymph.data._Data):
    
    def __init__(self, test_var=5):
        super().__init__()
        self.test_var = test_var

class TestDataMethods(unittest.TestCase):
    data=TestData()
    def test_data_assignment(self):
        self.assertEqual(self.data.test_var,5)

if __name__ == '__main__':
    unittest.main()
