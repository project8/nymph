'''
The file contains the base class for Python processors
'''

from nymph_bindings.processor import _Processor


class Processor(_Processor):
    '''
    Base class for all Python processors
    '''
    def __init__(self, name: str):
        '''
        Initialize things
        '''
        _Processor.__init__(self, name)
        print(f"Initializing python processor {name}")

    def configure(self, config):
        raise NotImplementedError( "Configure function has not been implemented for this processor" )
