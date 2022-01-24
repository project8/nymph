# CMakeLists for Nymph/Library
# Author: N.S. Oblath

#include( PythonPackage )

set( UTIL_DIR Utility )
set( DATA_DIR Data )
set( PROC_DIR Processor )
set( CONT_DIR Control )
set( IO_DIR IO )
set( APPL_DIR Application )

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}/${UTIL_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${DATA_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${CONT_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${IO_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${APPL_DIR}
)

set( NYMPH_HEADERFILES
    ${UTIL_DIR}/Exception.hh
    ${UTIL_DIR}/MemberVariable.hh

    ${DATA_DIR}/Data.hh
    ${DATA_DIR}/DataFrame.hh
    
    ${PROC_DIR}/PrimaryProcessor.hh
    ${PROC_DIR}/Processor.hh
    ${PROC_DIR}/ProcessorToolbox.hh
    ${PROC_DIR}/QuitChain.hh
    ${PROC_DIR}/SignalSlotBase.hh

    ${CONT_DIR}/ControlAccess.hh
    ${CONT_DIR}/Controller.hh
    ${CONT_DIR}/ControllerInterface.hh
    ${CONT_DIR}/ReturnBuffer.hh
    ${CONT_DIR}/SingleRunController.hh
)

set( NYMPH_SOURCEFILES
    ${UTIL_DIR}/Exception.cc

    ${DATA_DIR}/Data.cc
    ${DATA_DIR}/DataFrame.cc
    
    ${PROC_DIR}/PrimaryProcessor.cc
    ${PROC_DIR}/Processor.cc
    ${PROC_DIR}/ProcessorToolbox.cc
    ${PROC_DIR}/SignalSlotBase.cc


    ${CONT_DIR}/ControlAccess.cc
    ${CONT_DIR}/Controller.cc
    ${CONT_DIR}/ControllerInterface.cc
    ${CONT_DIR}/ReturnBuffer.cc
    ${CONT_DIR}/SingleRunController.cc
)


##################################################

pbuilder_library( 
    TARGET Nymph 
    SOURCES ${NYMPH_SOURCEFILES}
    PUBLIC_EXTERNAL_LIBRARIES ${PUBLIC_EXT_LIBS}
)

pbuilder_component_install_and_export( 
    COMPONENT Library
    LIBTARGETS Nymph
)

pbuilder_install_headers( ${NYMPH_HEADERFILES} )
