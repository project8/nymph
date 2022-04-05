
"""
  testprocessor.py
 
   Created on: Apr 05, 2022
       Author: F. Thomas
"""

__all__ = []

import sys

build_path = sys.argv[1]

sys.path.append(build_path + '/Scarab/python')
sys.path.append(build_path + '/Python/Bindings')

import _nymph
import scarab

class TestProcessor(_nymph.processor._Processor):
    
    def configure(self, param_node):
        
        param_dict = param_node.to_python()
        self.x = param_dict['x']

def main(args):
    
    testprocessor = TestProcessor('test')
    
    param = scarab.ParamValue( 1.0 )
    param_node = scarab.ParamNode()
    param_node.add('x', param)
    
    testprocessor.configure(param_node)
    
    print(testprocessor.x)
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
