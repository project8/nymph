# CMakeLists for Nymph/Python/Bindings
# Author: F. Thomas

set( NYMPH_BINDING_SOURCEFILES
    NymphPybind.cc
)

pybind11_add_module( _nymph ${NYMPH_BINDING_SOURCEFILES} )


