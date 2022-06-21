
"""
  testcontroller.py
 
   Created on: Jun 15, 2022
       Author: P. T. Surukuchi
"""
import unittest

import _nymph

class TestController(_nymph.control._Controller):
    
    def __init__(self, test_var1=5):
        super().__init__()
        self.test_var1 = test_var1

    def set_break_flag(flag):
        self.fBreakFlag=flag

    def set_canceled(flag):
        f_canceled.store(flag)
        
    def do_cancellation(flag):
        _Controller.do_cancellation(code)

class TestControllerMethod(unittest.TestCase):
    controller=TestController()
    def test_idata_assignment(self):
        self.controller.SetCycleTimeMS( 100 )
        self.assertEqual(controller.GetCycleTimeMS(),0)
        
if __name__ == '__main__':
    unittest.main()


