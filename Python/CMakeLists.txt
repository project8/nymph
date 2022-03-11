# CMakeLists.txt file for Nymph/Python
# Author: A. Ziegler
# Created on: Feb 4, 2022

if ( Nymph_ENABLE_PYTHON )

    message( "Building Python bindings")

    set( dir ${CMAKE_CURRENT_SOURCE_DIR} )

    # Python binding headers
    set( PYBINDING_HEADERFILES
        ${dir}/nymph/nymph_pybind.hh
    )

    # Python bindings
    set( PYBINDING_SOURCEFILES
        ${dir}/nymph/nymph_pybind.cc
    )

    set( PROJECT_LIBRARIES Nymph )

    pbuilder_add_pybind11_module( nymph ${PROJECT_LIBRARIES} )

    pbuilder_install_headers( ${PYBINDING_HEADERFILES} )

    message( "Done with Python bindings" )

endif( Nymph_ENABLE_PYTHON )