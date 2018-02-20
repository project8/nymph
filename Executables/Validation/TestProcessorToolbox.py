# TestProcessorToolbox.py
#  Created on: Jan 29, 2018
#      Author: N.S. Oblath
#
#  Tests the ability to use the processor toolbox from a Python script

import py_nymph as nymph
import py_nymph_validation as nv


ptb = nymph.KTProcessorToolbox('pt')

print('Configuring')

ptb.AddProcessor('test-p-proc', 'tpp')
ptb.AddProcessor('test-proc-b', 'tp')
ptb.MakeConnection('tpp:the-signal', 'tp:first-slot')
ptb.MakeConnection('tpp:the-signal', 'tp:first-slot')


ptb.PushBackToRunQueue('tpp')

print('Running')

ptb.Run()

print('All done')
