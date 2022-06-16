__all__ = []

import scarab
from _nymph.processor import SignalBase, SlotBase

__all__.append('Signal')
class Signal(SignalBase):
    '''
    Base class for all Python Signals
    '''
    def __init__(self, name, owner=None):
        '''
        Args:
            name (str) : the name of the signal, which specifies how a user will refer to the signal (e.g. in a config file) 
            owner (Processor) [optional] : reference to the object that will own the signal
        '''
        if owner == None:
            SignalBase.__init__(self, name)
        else:
            SignalBase.__init__(self, name, owner)

    def connect(self, slot, group):
        raise NotImplementedError('Python signals cannot yet be connected to slots')

    def emit(self):
        self()

    def __call__(self):
        raise NotImplementedError('Signal emission in Python has not yet been implemented')
