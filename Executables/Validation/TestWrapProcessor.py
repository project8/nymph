# TestWrapProcessor.py
#  Created on: Feb 1, 2018
#      Author: C. Claessens
#
#  Tests the ability to overwrite processor functions

import py_nymph as nymph
import py_nymph_validation as nv

class WrappedProcessor(nymph.WrapProcessor):
#    def WrapFunction(self, x):
#        self.fThreshold = 0
#        print("Hey, I'm a python processor. And by the way: fThreshold is {}".format(self.fThreshold))
    pass

print("Making instace in python of class WrappedProcessor that inherits from nymph.WrapProcessor")
p = WrappedProcessor()

print("Overwriting WrapFunction that will be called from the slot function")
p.WrapFunction = lambda x: print("Hey, I'm a python processor. And by the way: fThreshold is {}".format(p.fThreshold))

print("Adding a new variable to the processor")
p.fThreshold = 6

print('property fThreshold from python processor is {}'.format(p.fThreshold))
ptb = nymph.KTProcessorToolbox('pt')

print('Configuring')

ptb.AddProcessor('test-p-proc', 'tpp')
ptb.AddProcessor('bwp', p)
ptb.MakeConnection('tpp:the-signal', 'bwp:wrap-slot')


ptb.PushBackToRunQueue('tpp')

print('Running')

ptb.Run()

print('All done')