# Demonstrates access to KTPyTestClass

import py_nymph as nymph

# want to instantiate this will enough values to test all types: 
# # TypeError: get_value(): incompatible function arguments. The following argument types are supported:
#     1. (self: py_nymph.ParamNodePybind, arg0: str) -> bool
#     2. (self: py_nymph.ParamNodePybind, arg0: str) -> int
#     3. (self: py_nymph.ParamNodePybind, arg0: str) -> int
#     4. (self: py_nymph.ParamNodePybind, arg0: str) -> float
#     5. (self: py_nymph.ParamNodePybind, arg0: str) -> str
#     6. (self: py_nymph.ParamNodePybind, arg0: str, arg1: bool) -> bool
#     7. (self: py_nymph.ParamNodePybind, arg0: str, arg1: int) -> int
#     8. (self: py_nymph.ParamNodePybind, arg0: str, arg1: int) -> int
#     9. (self: py_nymph.ParamNodePybind, arg0: str, arg1: float) -> float
#     10. (self: py_nymph.ParamNodePybind, arg0: str, arg1: str) -> str

a_value = nymph.ParamValue(True)
print("Get a bool set to True: ", a_value.get())

# print(pnp.get_value())




a_node = nymph.ParamNode()
# Test the "get_value" function for all the different types: 
# a_node.get_value("myKey")