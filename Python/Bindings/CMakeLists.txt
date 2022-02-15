# CMakeLists for Nymph/Python/Bindings
# Author: F. Thomas

set( NYMPH_BINDING_SOURCEFILES
    NymphPybind.cc
)

set( lib_dependencies 
    Nymph
)

pybind11_add_module( _nymph ${NYMPH_BINDING_SOURCEFILES} )

target_link_libraries( _nymph PRIVATE Nymph )