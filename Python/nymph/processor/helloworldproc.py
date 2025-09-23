'''
Test Processor to demonstrate basic Python-based Nymph capabilities
'''

from . import processor

class HelloWorldPython( processor.Processor ):
    def __init__( self, name ):
        processor.Processor.__init__( self, name )

    def configure( self, config ):
        pass

    def say_hello():
        print( "Hello, world (python)" )
