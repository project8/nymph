# CMakeLists for Nymph/cpp/Library
# Author: N.S. Oblath

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
    ${UTIL_DIR}/Exception.hh
    ${UTIL_DIR}/MemberVariable.hh

    ${DATA_DIR}/Data.hh
    ${DATA_DIR}/DataFrame.hh
    
    ${PROC_DIR}/ControlAccess.hh
    ${PROC_DIR}/PrimaryProcessor.hh
    ${PROC_DIR}/Processor.hh
    ${PROC_DIR}/ProcessorToolbox.hh
    ${PROC_DIR}/QuitThread.hh
    ${PROC_DIR}/ReturnBuffer.hh
    ${PROC_DIR}/SignalSlotBase.hh

# Below are the v1 Library files

#    ${UTIL_DIR}/KTCacheDirectory.hh
#    ${UTIL_DIR}/KTConcurrentQueue.hh
#    ${UTIL_DIR}/KTConfigurable.hh
#    ${UTIL_DIR}/KTDirectory.hh
#    ${UTIL_DIR}/KTEventLoop.hh
#    ${UTIL_DIR}/KTException.hh
#    ${UTIL_DIR}/KTExtensible.hh
#    ${UTIL_DIR}/KTExtensibleStruct.hh
#    ${UTIL_DIR}/KTExtensibleStructFactory.hh
#    ${UTIL_DIR}/KTLogger.hh
#    ${UTIL_DIR}/KTMemberVariable.hh
#    ${UTIL_DIR}/KTTIFactory.hh
#    ${UTIL_DIR}/KTTime.hh
#    ${DATA_DIR}/KTApplyCut.hh
#    ${DATA_DIR}/KTCoreData.hh
#    ${DATA_DIR}/KTCut.hh
#    ${DATA_DIR}/KTCutFilter.hh
#    ${DATA_DIR}/KTCutResult.hh
#    ${DATA_DIR}/KTCutStatus.hh
#    ${DATA_DIR}/KTData.hh
#    ${DATA_DIR}/KTRegisterNymphExtData.hh
#    ${PROC_DIR}/KTConnection.hh
#    ${PROC_DIR}/KTPrimaryProcessor.hh
#    ${PROC_DIR}/KTProcessor.hh
#    ${PROC_DIR}/KTSignal.hh
#    ${PROC_DIR}/KTSignalWrapper.hh
#    ${PROC_DIR}/KTSlot.hh
#    ${PROC_DIR}/KTSlotWrapper.hh
#    ${PROC_DIR}/KTThreadReference.hh
#    ${IO_DIR}/KTJSONWriter.hh
#    ${IO_DIR}/KTReader.hh
#    ${IO_DIR}/KTWriter.hh
#    ${APPL_DIR}/KTApplication.hh
#    ${APPL_DIR}/KTCommandLineHandler.hh
#    ${APPL_DIR}/KTCommandLineOption.hh
#    ${APPL_DIR}/KTConfigurator.hh
#    ${APPL_DIR}/KTDataQueueProcessor.hh
#    ${APPL_DIR}/KTFilenameParsers.hh
#    ${APPL_DIR}/KTPrintDataStructure.hh
#    ${APPL_DIR}/KTProcessorToolbox.hh
#    ${APPL_DIR}/KTRunNymph.hh
#    ${APPL_DIR}/KTThroughputProfiler.hh
)

set( NYMPH_SOURCEFILES
    ${UTIL_DIR}/Exception.cc

    ${DATA_DIR}/Data.cc
    ${DATA_DIR}/DataFrame.cc
    
    ${PROC_DIR}/ControlAccess.cc
    ${PROC_DIR}/PrimaryProcessor.cc
    ${PROC_DIR}/Processor.cc
    ${PROC_DIR}/ProcessorToolbox.cc
    ${PROC_DIR}/ReturnBuffer.cc
    ${PROC_DIR}/SignalSlotBase.cc

# Below are the v1 Library files

#    ${UTIL_DIR}/KTCacheDirectory.cc
#    ${UTIL_DIR}/KTConfigurable.cc
#    ${UTIL_DIR}/KTDirectory.cc
#    ${UTIL_DIR}/KTEventLoop.cc
#    ${UTIL_DIR}/KTException.cc
#    ${UTIL_DIR}/KTTime.cc
#    ${DATA_DIR}/KTApplyCut.cc
#    ${DATA_DIR}/KTCoreData.cc
#    ${DATA_DIR}/KTCut.cc
#    ${DATA_DIR}/KTCutFilter.cc
#    ${DATA_DIR}/KTCutStatus.cc
#    ${DATA_DIR}/KTData.cc
#    ${PROC_DIR}/KTPrimaryProcessor.cc
#    ${PROC_DIR}/KTProcessor.cc
#    ${PROC_DIR}/KTSignalWrapper.cc
#    ${PROC_DIR}/KTSlotWrapper.cc
#    ${PROC_DIR}/KTThreadReference.cc
#    ${IO_DIR}/KTJSONWriter.cc
#    ${IO_DIR}/KTReader.cc
#    ${IO_DIR}/KTWriter.cc
#    ${APPL_DIR}/KTApplication.cc
#    ${APPL_DIR}/KTCommandLineHandler.cc
#    ${APPL_DIR}/KTConfigurator.cc
#    ${APPL_DIR}/KTDataQueueProcessor.cc
#    ${APPL_DIR}/KTFilenameParsers.cc
#    ${APPL_DIR}/KTPrintDataStructure.cc
#    ${APPL_DIR}/KTProcessorToolbox.cc
#    ${APPL_DIR}/KTRunNymph.cc
#    ${APPL_DIR}/KTThroughputProfiler.cc
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
