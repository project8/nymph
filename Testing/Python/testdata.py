"""
  testdata.py
 
   Created on: Jun 13, 2022
       Author: F. Thomas
"""

__all__ = []

import _nymph

class TestData(_nymph.data._Data):
    
    def __init__(self, test_var):
        super().__init__()
        self.test_var = test_var

def main(args):
    
    test_data = TestData('foo')
    
    print(test_data.test_var)
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))