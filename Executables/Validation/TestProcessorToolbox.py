# TestProcessorToolbox.py
#  Created on: Jan 29, 2018
#      Author: N.S. Oblath
#
#  Tests the ability to use the processor toolbox from a Python script

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
