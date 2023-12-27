
"""
  testdataframe.py
 
   Created on: Sep 10, 2023
       Author: N. S. Oblath
"""
import unittest

import _nymph

'''Data class with integer variables'''
class TestData1(_nymph.data._Data):
    
    def __init__(self, test_var1=0, test_var2=5):
        super().__init__()
        self.test_var1 = test_var1
        self.test_var2 = test_var2

'''Data class with floating point variables'''
class TestData2(_nymph.data._Data):
    
    def __init__(self, test_var1=0.0, test_var2=10.0):
        super().__init__()
        self.test_var1 = test_var1
        self.test_var2 = test_var2

# The way to access member variables need to be modified if getters/setters are used in the python version
class TestDataMethods(unittest.TestCase):
    dataframe = _nymph.DataFrame()


    '''Testing of integer type data'''
    data1=TestData1()
    def test_idata_assignment(self):
        self.assertEqual(self.data1.test_var1,0)
        self.assertEqual(self.data1.test_var2,5)
    
    '''Testing of floating point type data'''
    data2=TestData2()
    def test_fdata_assignment(self):
        self.assertAlmostEqual(self.data2.test_var1,0.0)
        self.assertAlmostEqual(self.data2.test_var2,10.0)

        #Change value and test
        self.data2.test_var2=20.0
        self.assertAlmostEqual(self.data2.test_var2,20.0)

if __name__ == '__main__':
    unittest.main()
