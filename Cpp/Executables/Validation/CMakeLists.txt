# CMakeLists for Nymph/Executables_v1/Validation
# Author: N. Oblath

############################
# Config files
############################

set (VALIDATION_CONFIGFILES
#    ConfigFiles/TestConfigurator.json
)

pbuilder_install_config (${VALIDATION_CONFIGFILES})


############################
# NymphValidation library files
############################

set (VALIDATION_HEADERFILES
#    KTTestConfigurable.hh
    TestCuts.hh
    TestData.hh
#    KTTestProcessor.hh
)

set (VALIDATION_SOURCEFILES
#    KTTestConfigurable.cc
    TestCuts.cc
    TestData.cc
#    KTTestProcessor.cc
)

set( LIB_DEPENDENCIES
    Nymph
)

############################
# NymphValidation library 
############################

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

pbuilder_library( 
    TARGET NymphValidation 
    SOURCES ${VALIDATION_SOURCEFILES}
    PROJECT_LIBRARIES ${LIB_DEPENDENCIES}
)

pbuilder_component_install_and_export( 
    COMPONENT Validation
    LIBTARGETS NymphValidation
)

pbuilder_install_headers( ${VALIDATION_HEADERFILES} )


############################
# Executables
############################

set( LIB_DEPENDENCIES
    NymphValidation
)

set( TEST_SOURCES
 #   TestApplication.cc
 #   TestApplyCut.cc
 #   TestCacheDirectory.cc
    TestCut.cc
    DataTest.cc
    TestCutFilter.cc
 #   TestLogger.cc
 #   TestPrintData.cc
 #   TestSignalsAndSlots.cc
 #   TestThroughputProfiler.cc
)

pbuilder_executables( TEST_PROGRAMS LIB_DEPENDENCIES )

set( TEST_PROGRAMS )
pbuilder_executables( 
    SOURCES ${TEST_SOURCES}
    TARGETS_VAR TEST_PROGRAMS
    PROJECT_LIBRARIES ${LIB_DEPENDENCIES}
    PUBLIC_EXTERNAL_LIBRARIES ${Nymph_EXE_LIBRARIES}
#        PRIVATE_EXTERNAL_LIBRARIES ${PRIVATE_EXT_LIBS}
)

pbuilder_component_install_and_export( 
    COMPONENT Validation
    EXETARGETS ${TEST_PROGRAMS}
)
