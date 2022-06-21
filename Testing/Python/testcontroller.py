
"""
  testcontroller.py
 
   Created on: Jun 15, 2022
       Author: P. T. Surukuchi
"""
import unittest

import _nymph

'''Data class with integer variables'''
class TestController(_nymph.control._Controller):
    
    def __init__(self, test_var1=5):
        super().__init__()
        self.test_var1 = test_var1

    def SetBreakFlag(flag):
        self.B=


class TestControllerMethod(unittest.TestCase):
    controller=TestController()
    def test_idata_assignment(self):
        self.assertEqual(self.data1.test_var1,0)

if __name__ == '__main__':
    unittest.main()




namespace Nymph
{
    class ControllerRevealer : public Controller
    {
        public:
            using Controller::Controller;
            virtual ~ControllerRevealer() {}

            void SetBreakFlag( bool flag )
            {
                fBreakFlag = flag;
                return;
            }

            void SetCanceled( bool flag )
            {
                f_canceled.store( flag );
                return;
            }

            void do_cancellation( int code )
            {
                Controller::do_cancellation( code );
                return;
            }
    };
}

