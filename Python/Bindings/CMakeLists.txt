# CMakeLists.txt file for Nymph/Python/Bindings
# Author: A. Ziegler
# Created on: Feb 4, 2022

message( "Building Python bindings")

set( TEST_DIR Testing/Cpp )

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Data
    ${CMAKE_CURRENT_SOURCE_DIR}/Processor
)

# Python binding headers
set( PYBINDING_HEADERFILES
    Data/DataPybind.hh
    Processor/ProcessorPybind.hh
    Processor/ProcessorRegistrar.hh
    Processor/PyProcCreatorPybind.hh
)

# Python bindings
set( PYBINDING_SOURCEFILES
    NymphPybind.cc
)

set( PROJECT_LIBRARIES Nymph )

pbuilder_add_pybind11_module( _nymph ${PROJECT_LIBRARIES} )

pbuilder_install_headers( ${PYBINDING_HEADERFILES} )

if( NOT DEFINED PBUILDER_PY_INSTALL_IN_SITELIB )
    message( STATUS "Installing add_lib_python_path.sh since python modules will not be installed in the site-package directory" )
    configure_file( add_lib_python_path.sh.in add_lib_python_path.sh @ONLY)
    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/add_lib_python_path.sh DESTINATION ${BIN_INSTALL_DIR} )
endif( NOT DEFINED PBUILDER_PY_INSTALL_IN_SITELIB )

message( "Done with Python bindings" )
