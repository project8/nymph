
import _nymph.processor as nproc

class TestProcessor(nproc._Processor):
    
    def configure(self, param_node):
        
        param_dict = param_node.to_python()
        self.x = param_dict['x']
        print('Configuring TestProcessor')

    def say_hi(self):
        print('Hi, I\'m Test Processor!')


if __name__ == "__main__":

    reg = nproc.register_py_processor( '', 'TestProcessor', 'test-proc' )

    proc = nproc.create_processor( "test-proc", "myproc" )
