# TestWrapProcessor.py
#  Created on: Feb 1, 2018
#      Author: C. Claessens
#
#  Tests the ability to overwrite processor functions

import py_nymph as nymph
import py_nymph_validation as nv

class WrappedProcessor(nymph.WrapProcessor):
    def __init__(self):
        super(WrappedProcessor, self).__init__()
    pass


ptb = nymph.KTProcessorToolbox('pt')

print('Configuring')

ptb.AddProcessor('test-p-proc', 'tpp')
#ptb.AddProcessor('test-proc-b', 'tp')

p = WrappedProcessor()
p.WrapFunction = lambda x: print("Hey, I'm python")
print('type of WrappedProcessor in python is: {}'.format( type(p)))


ptb.AddProcessor('bwp', p)
ptb.MakeConnection('tpp:the-signal', 'bwp:wrap-slot')


ptb.PushBackToRunQueue('tpp')

print('Running')

ptb.Run()

print('All done')