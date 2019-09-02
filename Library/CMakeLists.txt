# CMakeLists for Nymph/Library
# Author: N.S. Oblath

include( PythonPackage )

set( UTIL_DIR Utility )
set( DATA_DIR Data )
set( PROC_DIR Processor )
set( IO_DIR IO )
set( APPL_DIR Application )

set( NYMPH_HEADERFILES
    ${UTIL_DIR}/Exception.hh
    ${UTIL_DIR}/MemberVariable.hh
)

set( NYMPH_SOURCEFILES
    ${UTIL_DIR}/Exception.cc
)


##################################################

pbuilder_library( Nymph NYMPH_SOURCEFILES "" )
pbuilder_install_headers( ${NYMPH_HEADERFILES} )
