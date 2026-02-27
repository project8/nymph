# Demonstrates access to param classes

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


# Value testing
int_value = nymph.param_value(5)
print("What type of value is stored? (bool, uint, int, double, string): ", int_value.is_bool(), int_value.is_uint(), int_value.is_int(), int_value.is_double(), int_value.is_string())
print("Get value set to 5:", int_value.as_int())

string_value = nymph.param_value("Hello!")
print("What type of value is stored? (bool, uint, int, double, string): ", string_value.is_bool(), string_value.is_uint(), string_value.is_int(), string_value.is_double(), string_value.is_string())
print("Get value set to \"Hello!\":", string_value.as_string())


# Node testing
a_node = nymph.param_node()
a_node.add("int-value", int_value)
a_node.add("string-value", string_value)

print("Int value from node:", a_node.get_value_int("int-value"))
print("String value from node:", a_node.get_value_string("string-value"))

# Test the "get_value" function for all the different types: 
print("Get int value from the node as different types (bool, uint, int, double, string): ", a_node.get_value_bool("int-value"), a_node.get_value_uint("int-value"), a_node.get_value_int("int-value"), a_node.get_value_double("int-value"), a_node.get_value_string("int-value"))


# Array testing
an_array = nymph.param_array()
an_array.resize(2)
an_array.assign(0, int_value)
an_array.assign(1, string_value)

print("Int value from array:", an_array.get_value_int(0))
print("String value from array:", an_array.get_value_string(1))

# Test the "get_value" function for all the different types: 
print("Get int value from the array as different types (bool, uint, int, double, string): ", an_array.get_value_bool(0), an_array.get_value_uint(0), an_array.get_value_int(0), an_array.get_value_double(0), an_array.get_value_string(0))
