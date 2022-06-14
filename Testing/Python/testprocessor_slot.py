
"""
  testprocessor.py
 
   Created on: Apr 05, 2022
       Author: F. Thomas
"""

__all__ = []

import _nymph
import scarab


class Slot():
    def __init__(self, function, name):
        self.function = function
        self.name = name

    def execute(self):
        self.function()


class TestProcessor(_nymph.processor._Processor):

    def __init__(self, name):
        _nymph.processor._Processor.__init__(self, name)
        self.do_something_slot = Slot(self.do_something, 'do_something')

    def configure(self, param_node):
        
        param_dict = param_node.to_python()
        self.x = param_dict['x']

    def do_something(self):
        print('Do something')



def main(args):
    
    testprocessor = TestProcessor('test')
    test2 = _nymph.processor.get_processor('test-proc', 'name')
    
    param = scarab.ParamValue( 1.0 )
    param_node = scarab.ParamNode()
    param_node.add('x', param)

    testprocessor.configure(param_node)
    print(testprocessor.x)

    testprocessor.do_something()
    testprocessor.do_something_slot.execute()

    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
