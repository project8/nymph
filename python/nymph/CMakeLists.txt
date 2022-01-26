# CMakeLists for Nymph/python/Library
# Author: N.S. Oblath

#include( PythonPackage )

set( UTIL_DIR Utility )
set( DATA_DIR Data )
set( PROC_DIR Processor )
set( IO_DIR IO )
set( APPL_DIR Application )

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}/${UTIL_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${DATA_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${IO_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${APPL_DIR}
)

set( NYMPH_HEADERFILES

)

set( NYMPH_SOURCEFILES

)

##################################################


# Python binding headers
set( NYMPH_PYBINDING_HEADERFILES
    #${UTIL_DIR}/KTConfigurablePybind.hh
    ${UTIL_DIR}/KTParamPybind.hh
    ${PROC_DIR}/KTProcessorPybind.hh
    ${PROC_DIR}/KTPyProcessor.hh
)

# Python bindings
set( NYMPH_PYBINDING_SOURCEFILES
    ${PROC_DIR}/KTProcessorPybind.cc
    NymphPybind.cc
)

##################################################

pbuilder_library( Nymph NYMPH_SOURCEFILES "" )
pbuilder_install_headers( ${NYMPH_HEADERFILES} )

if( Nymph_ENABLE_PYTHON )
    set( LIB_DEPENDENCIES Nymph )

    configure_file( AddLibPythonPath.sh.in AddLibPythonPath.sh @ONLY)
    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/AddLibPythonPath.sh DESTINATION ${BIN_INSTALL_DIR} )
    
    pybind11_add_module( py_nymph MODULE ${NYMPH_PYBINDING_SOURCEFILES} )
    target_link_libraries( py_nymph PRIVATE ${LIB_DEPENDENCIES} ${EXTERNAL_LIBRARIES} )
    pbuilder_install_libraries( py_nymph )
    pbuilder_install_headers( ${NYMPH_PYBINDING_HEADERFILES} )
endif( Nymph_ENABLE_PYTHON )