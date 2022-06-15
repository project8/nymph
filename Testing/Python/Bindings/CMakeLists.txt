# CMakeLists.txt file for Nymph/Testing/Python/Bindings
# Author: N.S. Oblath
# Created on: Jun 14, 2022

message( "Building Python bindings for NymphTesting")

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Python binding headers
set( TESTING_PYBINDING_HEADERFILES
)

# Python bindings
set( PYBINDING_SOURCEFILES
    NymphTestingPybind.cc
)

set( PROJECT_LIBRARIES Nymph NymphTesting )

pbuilder_add_pybind11_module( _nymph_testing ${PROJECT_LIBRARIES} )

# Currently there are no headers
#pbuilder_install_headers( ${TESTING_PYBINDING_HEADERFILES} )

# This is done in the main Nymph pybinding
#if( NOT DEFINED PBUILDER_PY_INSTALL_IN_SITELIB )
#    message( STATUS "Installing add_lib_python_path.sh since python modules will not be installed in the site-package directory" )
#    configure_file( add_lib_python_path.sh.in add_lib_python_path.sh @ONLY)
#    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/add_lib_python_path.sh DESTINATION ${BIN_INSTALL_DIR} )
#endif( NOT DEFINED PBUILDER_PY_INSTALL_IN_SITELIB )

message( "Done with Testing Python bindings" )
