
"""
  testchain.py
 
   Created on: Jun 12, 2022
       Author: F. Thomas
"""

__all__ = []

import nymph
import katydid

"""setting up a fixed analysis (similar to yaml)"""

#declaring processors
egg = katydid.Egg(filename='/path/to/file.egg',
                    egg_reader='egg3',
                    slice_size=16384,
                    number_of_slices=1)
                    
fft = katydid.ForwardFFTW(transform_flag='ESTIMATE')

to_ps = katydid.ConvertToPower()

writer = katydid.BasicRootWriter(output_file='/path/to/file.root',
                                    file_flag='recreate')
                                    
#making connections
nymph.connect(egg.header, fft.header)
nymph.connect(egg.ts, fft.ts_fftw)
nymph.connect(fft.fft, to_ps.fs_fftw_-to_psd)
nymph.connect(to_ps.psd, writer.ps)

#create run queue
run_queue = nymph.RunQueue(egg)

"""Now some interactive use after the analysis chain was declared"""

#trigger the analysis run to create 'some_data'
nymph_data = run_queue.get('some_data')

transformed_data = python_function(nymph_data)

#use the transformed data with some new python processor
python_processor = TestProcessor(param_a='foo', param_b='bar')
python_processor.do_something(transformed_data)
