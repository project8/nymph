'''
The file contains the pure-Python Signal class
'''

from nymph_bindings.processor import _SignalBase


class PurePySignal(_SignalBase):
    '''
    Signals that are pure Python

    These cannot be connected to C++ or C++-backed Slots
    '''
    def __init__(self):
        '''
        Initialize things
        '''
        _SignalBase.__init__(self)

    # To implement:
    #   - Connect
    #   - emit
    #   - __call__
    