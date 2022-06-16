__all__ = []

import scarab
from _nymph.processor import SignalBase, SlotBase

__all__.append('Slot')
class Slot(SlotBase):
    '''
    Base class for all Python Slot
    '''
    def __init__(self, name, func, owner=None):
        '''
        Args:
            name (str) : the name of the slot, which specifies how a user will refer to the slot (e.g. in a config file) 
            owner (Processor) [optional] : reference to the object that will own the slot
        '''
        if owner == None:
            SlotBase.__init__(self, name)
        else:
            SlotBase.__init__(self, name, owner)

        self.func = func

    def connect_to(self, slot, group):
        raise NotImplementedError('Python slots cannot yet be connected to signals')

    def __call__(self):
        raise NotImplementedError('Slot calls in Python has not yet been implemented')

    # TODO? DoBreakpoint
