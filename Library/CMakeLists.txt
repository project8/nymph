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
    
    ${PROC_DIR}/ControlAccess.hh
    ${PROC_DIR}/PrimaryProcessor.hh
    ${PROC_DIR}/Processor.hh
    ${PROC_DIR}/ProcessorToolbox.hh
    ${PROC_DIR}/SignalSlotBase.hh
)

set( NYMPH_SOURCEFILES
    ${UTIL_DIR}/Exception.cc

    ${PROC_DIR}/ControlAccess.cc
    ${PROC_DIR}/PrimaryProcessor.cc
    ${PROC_DIR}/Processor.cc
    ${PROC_DIR}/ProcessorToolbox.cc
    ${PROC_DIR}/SignalSlotBase.cc
)


##################################################

pbuilder_library( Nymph NYMPH_SOURCEFILES "" )
pbuilder_install_headers( ${NYMPH_HEADERFILES} )
