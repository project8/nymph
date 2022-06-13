#
#TestData.py
#
#  Created on: June 13, 2022
#  Author: P. T. Surukuchi
#

import unittest

import _nymph

# class TestDataMethods(unittest.TestCase):

#     def setUp(self):
#         self.data = _nymph.data._Data()

#     def __init__(self, test_var):
#         super().__init__()

#     def test_data1(self):
#         self.assertEqual(fIValue1,5)

#     def test_isupper(self):
#         self.assertTrue('FOO'.isupper())
#         self.assertFalse('Foo'.isupper())

#     def test_split(self):
#         s = 'hello world'
#         self.assertEqual(s.split(), ['hello', 'world'])
#         # check that s.split fails when the separator is not a string
#         with self.assertRaises(TypeError):
#             s.split(2)


class TestData(_nymph.data._Data):
    
    def __init__(self, test_var=5):
        super().__init__()
        self.test_var = test_var

class TestDataMethods(unittest.TestCase):
    def TestDataVal(self):
        test_data = TestData('foo')
        self.assertEqual(test_data.GetIValue1(),5)

if __name__ == '__main__':
    unittest.main()
