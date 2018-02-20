# CMakeLists for Nymph/Executables/Validation
# Author: N. Oblath

if (Nymph_ENABLE_TESTING)

    # Config files
    set (VALIDATION_CONFIGFILES
        ConfigFiles/TestConfigurator.json
    )
    
    pbuilder_install_config (${VALIDATION_CONFIGFILES})
    

    # Headers for any test classes
    set (VALIDATION_HEADERFILES
        KTJSONReader.hh
        KTJSONWriterValidation.hh
        KTTestConfigurable.hh
        KTTestCuts.hh
        KTTestData.hh
        KTTestDataPrimaryProcessor.hh
        KTTestPrimaryProcessor.hh
        KTTestProcessor.hh
    )
    
    set (VALIDATION_SOURCEFILES
        KTJSONReader.cc
        KTJSONWriterValidation.cc
        KTTestConfigurable.cc
        KTTestCuts.cc
        KTTestData.cc
        KTTestDataPrimaryProcessor.cc
        KTTestPrimaryProcessor.cc
        KTTestProcessor.cc
    )
    
    set( LIB_DEPENDENCIES
        Nymph
    )
    
    
    pbuilder_library( NymphValidation VALIDATION_SOURCEFILES LIB_DEPENDENCIES )
    pbuilder_install_headers (${VALIDATION_HEADERFILES})
    
 
    # Executables
    
    set( LIB_DEPENDENCIES
        Nymph 
        NymphValidation
    )
    
    set( TEST_PROGRAMS
        TestApplication
        TestApplyCut
        TestCacheDirectory
        TestCut
        TestCutFilter
        TestData
        TestExceptionInSlot
        TestExt
        TestExtSerialization
        TestJSONWriter
        TestJSONWriterViaSlot
        TestLogger
        TestPolymorphicSlotData
        TestPrimaryProcessor
        TestPrimaryProcessorNoThreading
        TestPrintData
        TestProcessorToolbox2Threads
        TestProcessorToolboxMultithreaded
        TestProcessorToolboxSinglethreaded
        TestSerialization
        TestSignalsAndSlots
        TestSlotData
        TestThroughputProfiler
        TestUseBreakpoint
    )
    
    pbuilder_executables( TEST_PROGRAMS LIB_DEPENDENCIES )
        
endif (Nymph_ENABLE_TESTING) 
