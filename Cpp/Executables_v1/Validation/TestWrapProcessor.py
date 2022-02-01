# TestWrapProcessor.py
#  Created on: Feb 1, 2018
#      Author: C. Claessens
#
#  Tests the ability to overwrite processor functions

import py_nymph as nymph
import py_nymph_validation as nv

class WrappedProcessor(nymph.WrapProcessor):
    pass

print("\nMaking instace in python of class WrappedProcessor that inherits from nymph.WrapProcessor.\n")
p = WrappedProcessor()


ptb = nymph.KTProcessorToolbox('pt')

print('Configuring')
ptb.AddProcessor('test-p-proc', 'tpp')
ptb.AddProcessor('bwp', p)
ptb.MakeConnection('tpp:the-signal', 'bwp:wrap-slot')

ptb.PushBackToRunQueue('tpp')


print("\nBefore running: Overwrite WrapFunction that will be called from the slot function.\n")
p.WrapFunction = lambda x: print("Hey, I'm a python processor. And by the way: fThreshold is {}".format(p.fThreshold))


print("Add a new variable to the processor.\n")
p.fThreshold = 6
print('Property fThreshold from python processor is {}.\n'.format(p.fThreshold))


print('Define new Configure method to set self.fThreshold.\n')
def Configure(self, a):
    print('In Configure: Setting new fThreshold.\n')
    self.fThreshold = a
    
p.Configure = Configure
p.Configure(p, 7)




print('Running')

ptb.Run()

print('All done')