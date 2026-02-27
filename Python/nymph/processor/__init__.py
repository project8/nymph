
from nymph_bindings.processor import *

from .helloworldproc import *
from .processor import *
from .registrar import *

register("nymph.processor", "HelloWorldPython", "hello-world-py")
