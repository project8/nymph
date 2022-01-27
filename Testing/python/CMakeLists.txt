# CMakeLists for Nymph/Testing/python
# Author: N. Oblath

############################
# Config files
############################

set (TESTING_CONFIGFILES
    ConfigFiles/TestConfigurator.json
)

pbuilder_install_config (${TESTING_CONFIGFILES})


############################
# NymphPythonTesting library files
############################

set (PYTHONTESTING_HEADERFILES
    KTPyTestClass.hh
    KTPyTestClassPybind.hh
    KTTestPythonBasics.hh
)

set (PYTHONTESTING_SOURCEFILES
    KTPyTestClass.cc
    KTPyTestClassPybind.cc
    KTTestPythonBasics.cc
)

set( LIB_DEPENDENCIES
    Nymph
)

############################
# NymphPythonTesting library 
############################

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

pbuilder_library( 
    TARGET NymphPythonTesting
    SOURCES ${PYTHONTESTING_SOURCEFILES}
    PROJECT_LIBRARIES ${LIB_DEPENDENCIES}
)

pbuilder_component_install_and_export( 
    COMPONENT PythonTesting
    LIBTARGETS NymphPythonTesting
)

pbuilder_install_headers( ${PYTHONTESTING_HEADERFILES} )


############################
# Executables
############################

set( LIB_DEPENDENCIES
    NymphPythonTesting
)

#
#set( TEST_SOURCES
#    TestApplication.cc
#    TestApplyCut.cc
#    TestCacheDirectory.cc
#    TestCut.cc
#    TestCutFilter.cc
#    TestLogger.cc
#    TestPrintData.cc
#    TestSignalsAndSlots.cc
#    TestThroughputProfiler.cc
#)

#pbuilder_executables( TEST_PROGRAMS LIB_DEPENDENCIES )

#set( TEST_PROGRAMS )
#pbuilder_executables( 
#    SOURCES ${TEST_SOURCES}
#    TARGETS_VAR TEST_PROGRAMS
#    PROJECT_LIBRARIES ${LIB_DEPENDENCIES}
#    PUBLIC_EXTERNAL_LIBRARIES ${Nymph_EXE_LIBRARIES}
#        PRIVATE_EXTERNAL_LIBRARIES ${PRIVATE_EXT_LIBS}
#)

#pbuilder_component_install_and_export( 
#    COMPONENT Validation
#    EXETARGETS ${TEST_PROGRAMS}
#)
