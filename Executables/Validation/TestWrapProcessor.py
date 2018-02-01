# TestWrapProcessor.py
#  Created on: Feb 1, 2018
#      Author: C. Claessens
#
#  Tests the ability to overwrite processor functions

import py_nymph as nymph
import py_nymph_validation as nv

class WrappedProcessor(nv.WrapProcessor):
    pass

ptb = nymph.KTProcessorToolbox('pt')

print('Configuring')

ptb.AddProcessor('test-p-proc', 'tpp')
ptb.AddProcessor('test-proc-b', 'tp')
ptb.AddProcessor('base-wrap-proc', 'bwp')



#ptb.MakeConnection('tpp:the-signal', 'tp:first-slot')
#ptb.MakeConnection('tpp:the-signal', 'tp:first-slot')
ptb.MakeConnection('tpp:the-signal', 'bwp:wrap-slot')

p = WrappedProcessor()
p.WrapFunction = lambda x: print("Hey, I'm python")

ptb.PushBackToRunQueue('tpp')

print('Running')

ptb.Run()

print('All done')