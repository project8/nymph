
"""
  testsignal.py
 
   Created on: Jun 14, 2022
       Author: F. Thomas
"""

__all__ = []

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
    
    signal = _nymph.processor._SignalData('test-signal', testprocessor)
    
    print(signal.name)
    print(testprocessor.x)
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
