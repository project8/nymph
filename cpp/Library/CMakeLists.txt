# CMakeLists for Nymph/Library
# Author: N. Oblath

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
    ${UTIL_DIR}/KTCacheDirectory.hh
    ${UTIL_DIR}/KTConcurrentQueue.hh
    ${UTIL_DIR}/KTConfigurable.hh
    ${UTIL_DIR}/KTDirectory.hh
    ${UTIL_DIR}/KTEventLoop.hh
    ${UTIL_DIR}/KTException.hh
    ${UTIL_DIR}/KTExtensibleStruct.hh
    ${UTIL_DIR}/KTExtensibleStructFactory.hh
    ${UTIL_DIR}/KTLogger.hh
    ${UTIL_DIR}/KTMemberVariable.hh
    ${UTIL_DIR}/KTTIFactory.hh
    ${UTIL_DIR}/KTTime.hh
    ${DATA_DIR}/KTApplyCut.hh
    ${DATA_DIR}/KTCut.hh
    ${DATA_DIR}/KTCutFilter.hh
    ${DATA_DIR}/KTCutResult.hh
    ${DATA_DIR}/KTCutStatus.hh
    ${DATA_DIR}/KTData.hh
    ${PROC_DIR}/KTConnection.hh
    ${PROC_DIR}/KTPrimaryProcessor.hh
    ${PROC_DIR}/KTProcessor.hh
    ${PROC_DIR}/KTSignal.hh
    ${PROC_DIR}/KTSignalWrapper.hh
    ${PROC_DIR}/KTSlot.hh
    ${PROC_DIR}/KTSlotWrapper.hh
    ${IO_DIR}/KTReader.hh
    ${IO_DIR}/KTWriter.hh
    ${APPL_DIR}/KTApplication.hh
    ${APPL_DIR}/KTCommandLineHandler.hh
    ${APPL_DIR}/KTCommandLineOption.hh
    ${APPL_DIR}/KTConfigurator.hh
    ${APPL_DIR}/KTDataQueueProcessor.hh
    ${APPL_DIR}/KTFilenameParsers.hh
    ${APPL_DIR}/KTPrintDataStructure.hh
    ${APPL_DIR}/KTProcessorToolbox.hh
    ${APPL_DIR}/KTRunNymph.hh
    ${APPL_DIR}/KTThroughputProfiler.hh
)

set( NYMPH_SOURCEFILES
    ${UTIL_DIR}/KTCacheDirectory.cc
    ${UTIL_DIR}/KTConfigurable.cc
    ${UTIL_DIR}/KTDirectory.cc
    ${UTIL_DIR}/KTEventLoop.cc
    ${UTIL_DIR}/KTException.cc
    ${UTIL_DIR}/KTLogger.cc
    ${UTIL_DIR}/KTTime.cc
    ${DATA_DIR}/KTApplyCut.cc
    ${DATA_DIR}/KTCut.cc
    ${DATA_DIR}/KTCutFilter.cc
    ${DATA_DIR}/KTCutStatus.cc
    ${DATA_DIR}/KTData.cc
    ${PROC_DIR}/KTPrimaryProcessor.cc
    ${PROC_DIR}/KTProcessor.cc
    ${PROC_DIR}/KTSignal.cc
    ${PROC_DIR}/KTSignalWrapper.cc
    ${PROC_DIR}/KTSlotWrapper.cc
    ${IO_DIR}/KTReader.cc
    ${IO_DIR}/KTWriter.cc
    ${APPL_DIR}/KTApplication.cc
    ${APPL_DIR}/KTCommandLineHandler.cc
    ${APPL_DIR}/KTConfigurator.cc
    ${APPL_DIR}/KTDataQueueProcessor.cc
    ${APPL_DIR}/KTFilenameParsers.cc
    ${APPL_DIR}/KTPrintDataStructure.cc
    ${APPL_DIR}/KTProcessorToolbox.cc
    ${APPL_DIR}/KTRunNymph.cc
    ${APPL_DIR}/KTThroughputProfiler.cc
)

#set( NYMPH_PY_HEADERFILES
#    ${DATA_DIR}/KTDataPy.hh
#    ${PROC_DIR}/KTProcessorPy.hh
#    ${APPL_DIR}/KTProcessorToolboxPy.hh
#)

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


#if (Nymph_ENABLE_PYTHON)
#    python_package_add_module(NymphPy.cc)
#endif (Nymph_ENABLE_PYTHON)
