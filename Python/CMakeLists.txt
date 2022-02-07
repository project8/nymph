# CMakeLists.txt file for Nymph/Python
# Author: A. Ziegler
# Created on: Feb 4, 2022

if( Nymph_ENABLE_PYTHON)
    message( "Building Python bindings")

    set( dir ${CMAKE_CURRENT_SOURCE_DIR} )

    # Python binding headers
    set( PYBINDING_HEADERFILES

    )

    # Python bindings
    set( PYBINDING_SOURCEFILES

    )

    set( PROJECT_LIBRARIES Scarab )

    pbuilder_add_pybind11_module( scarab ${PROJECT_LIBRARIES} )

    pbuilder_install_headers( ${PYBINDING_HEADERFILES} )z

    message( "Done with Python bindings" )
endif (Nymph_ENABLE_PYTHON)