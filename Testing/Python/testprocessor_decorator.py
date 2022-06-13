
"""
  testprocessor.py
 
   Created on: Apr 05, 2022
       Author: F. Thomas
"""

__all__ = []

import _nymph
import scarab


class Decorators():
    def Slot(method_to_decorate):
        def execute(self):
            print('This is a slot method')
            method_to_decorate(self)
        return execute

    def Signal(method_to_decorate):
        def execute(self):
            print('This is a signal method')
            method_to_decorate(self)
        return execute

class TestProcessor(_nymph.processor._Processor, Decorators):
    
    def configure(self, param_node):
        
        param_dict = param_node.to_python()
        self.x = param_dict['x']

    @Decorators.Slot
    def do_something(self):
        print('Do something')

    @Decorators.Signal
    def do_another_thing(self):
        print('Do another thing')


def main(args):
    
    testprocessor = TestProcessor('test')
    test2 = _nymph.processor.get_processor('test-proc', 'name')
    
    param = scarab.ParamValue( 1.0 )
    param_node = scarab.ParamNode()
    param_node.add('x', param)

    testprocessor.configure(param_node)
    print(testprocessor.x)

    testprocessor.do_something()
    testprocessor.do_another_thing()

    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
