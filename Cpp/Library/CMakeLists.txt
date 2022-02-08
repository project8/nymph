# CMakeLists for Nymph/Cpp/Library
# Author: N.S. Oblath

set( CONT_DIR Control )
set( DATA_DIR Data )
set( IMPL_DIR Implementation )
#set( IO_DIR IO )
set( PROC_DIR Processor )
set( UTIL_DIR Utility )

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}/${CONT_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${DATA_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${IMPL_DIR}
#    ${CMAKE_CURRENT_SOURCE_DIR}/${IO_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${UTIL_DIR}
)

set( NYMPH_HEADERFILES
    ${UTIL_DIR}/Exception.hh
    ${UTIL_DIR}/MemberVariable.hh

    ${PROC_DIR}/PrimaryProcessor.hh
    ${PROC_DIR}/Processor.hh
    ${PROC_DIR}/ProcessorToolbox.hh
    ${PROC_DIR}/QuitChain.hh
    ${PROC_DIR}/Signal.hh
    ${PROC_DIR}/SignalSlotBase.hh
    ${PROC_DIR}/Slot.hh

    ${DATA_DIR}/Data.hh
    ${DATA_DIR}/DataFrame.hh
    ${DATA_DIR}/DataPresent.hh
    ${DATA_DIR}/SignalData.hh
    ${DATA_DIR}/SlotData.hh
    
    ${CONT_DIR}/ControlAccess.hh
    ${CONT_DIR}/Controller.hh
    ${CONT_DIR}/ControllerInterface.hh
    ${CONT_DIR}/ReturnBuffer.hh
    ${CONT_DIR}/SingleRunController.hh

    ${IMPL_DIR}/RunNymph.hh
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

    ${IMPL_DIR}/RunNymph.cc
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
