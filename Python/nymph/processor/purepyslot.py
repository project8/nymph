'''
The file contains the pure-Python Slot class
'''

from nymph_bindings.processor import _SlotBase


class PurePySlot(_SlotBase):
    '''
    Slots that are pure Python

    These cannot be connected to C++ or C++-backed Signals
    '''
    def __init__(self):
        '''
        Initialize things
        '''
        _SlotBase.__init__(self)

    # To implement:
    #   - ConnectTo
    #   - MatchesTo
    #   - holds function to call
