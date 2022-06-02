# CMakeLists.txt file for Nymph/Python
# Author: A. Ziegler
# Created on: Feb 4, 2022

if ( Nymph_ENABLE_PYTHON )

    message( "Building Python bindings")

    set( dir ${CMAKE_CURRENT_SOURCE_DIR} )

    # Python binding headers
    set( PYBINDING_HEADERFILES
        ${dir}/nymph/nymph_pybind_template.hh
    )

    # Python bindings
    set( PYBINDING_SOURCEFILES
        ${dir}/nymph/nymph_pybind_template.cc
    )

    set( PROJECT_LIBRARIES Nymph )

    pbuilder_add_pybind11_module( nymph ${PROJECT_LIBRARIES} )

    pbuilder_install_headers( ${PYBINDING_HEADERFILES} )

    if( NOT DEFINED PBUILDER_PY_INSTALL_IN_SITELIB )
        message( STATUS "Installing add_lib_python_path.sh since python modules will not be installed in the site-package directory" )
        configure_file( nymph/add_lib_python_path.sh.in nymph/add_lib_python_path.sh @ONLY)
        install( FILES ${CMAKE_CURRENT_BINARY_DIR}/nymph/add_lib_python_path.sh DESTINATION ${BIN_INSTALL_DIR} )
    endif( NOT DEFINED PBUILDER_PY_INSTALL_IN_SITELIB )

    message( "Done with Python bindings" )

endif( Nymph_ENABLE_PYTHON )