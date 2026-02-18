
"""
  testsignalslot.py
 
   Created on: Oct 10, 2025
       Author: N.S. Oblath
"""

__all__ = []

import unittest

import nymph_bindings
import scarab


def add_one(value: int):
    

class TestIntSignalSlot(unittest.TestCase):
    def test_connection(self):
        int_sig = nymph_bindings.processor.IntSignal("test_sig")
        int_slot = nymph_bindings.processor.IntSlot("test_slot")





#class TestProcessorBroken(nymph_bindings.processor._Processor):
#    """Should not work because it does not override configure"""
#    
#    def foo():
#        print('I am a failed processor implementation')
#        
#
#class TestProcessor(nymph_bindings.processor._Processor):
#    
#    def configure(self, param_node):
#        
#        param_dict = param_node.to_python()
#        
#        self.value = param_dict['value']
#        
#        if 'foo' in param_dict:
#            self.foo = param_dict['foo']
#            
#        #todo add test of a configexception thing
#
#
#class TestPureVirtual(unittest.TestCase):
#    
#    def test_pure_virtual(self):
#        test_proc = TestProcessorBroken('test-proc')
#        
#        with self.assertRaises(RuntimeError) as context:
#            test_proc.configure(scarab.to_param({}))
#
#        self.assertTrue(str(context.exception) == 'Tried to call pure virtual function "Nymph::Processor::configure"')
#        
#
#class TestConfigure(unittest.TestCase):
#    
#    def test_configure(self):
#        test_proc = TestProcessor('test-proc')
#        test_proc.configure(scarab.to_param({'value': 42, 'foo': 'bar'}))
#        
#        self.assertEqual(test_proc.value, 42)
#        self.assertEqual(test_proc.foo, 'bar')
#        self.assertEqual(test_proc.name, 'test-proc')
#
#
#class TestPyProcCreator(unittest.TestCase):
#    
#    def test_creating(self):
#        
#        registrar = nymph_bindings.processor._register('testprocessor', 'TestProcessor', 'processor-name')
#        test_proc = nymph_bindings.processor._create('processor-name', 'test-proc')
#        
#        self.assertEqual(test_proc.name, 'test-proc')
#
#
#if __name__ == '__main__':
#    unittest.main()
#
#