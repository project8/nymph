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
        KTPyTestClass.hh
        KTTestConfigurable.hh
        KTTestCuts.hh
        KTTestData.hh
        KTTestPrimaryProcessor.hh
        KTTestProcessor.hh
    )
    
    set (VALIDATION_SOURCEFILES
        KTPyTestClass.cc
        KTTestConfigurable.cc
        KTTestCuts.cc
        KTTestData.cc
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
        TestExceptionInSlot
        TestLogger
        TestPolymorphicSlotData
        TestPrimaryProcessor
        TestPrimaryProcessorNoThreading
        TestPrintData
        TestProcessorToolbox2Threads
        TestProcessorToolboxMultithreaded
        TestProcessorToolboxSinglethreaded
        TestSignalsAndSlots
        TestSlotData
        TestThroughputProfiler
        TestUseBreakpoint
    )
    
    pbuilder_executables( TEST_PROGRAMS LIB_DEPENDENCIES )
    
    
    if( Nymph_ENABLE_PYBIND11 )
        # Python Interface
        
        # Python binding headers
        set( VALIDATION_PYBINDING_HEADERFILES
            KTPyTestClassPybind.hh
            TestPythonBasics.hh
        )
        
        # Python bindings
        set( VALIDATION_PYBINDING_SOURCEFILES
            NymphValidationPybind.cc
        )
    
        # Python scripts
        set( VALIDATION_PY_SCRIPTS
            TestProcessorToolbox.py
            TestPyTestClass.py
            TestPythonBasics.py
            TestPythonBindings.py
        )
        
        pybind11_add_module( py_nymph_validation MODULE ${VALIDATION_PYBINDING_SOURCEFILES} )
        target_link_libraries( py_nymph_validation PRIVATE ${LIB_DEPENDENCIES} ${EXTERNAL_LIBRARIES} )
        pbuilder_install_libraries( py_nymph_validation )
        pbuilder_install_headers( ${VALIDATION_PYBINDING_HEADERFILES} )
        
        install( FILES ${VALIDATION_PY_SCRIPTS} DESTINATION ${BIN_INSTALL_DIR} )
    
    endif( Nymph_ENABLE_PYBIND11 )
        
endif (Nymph_ENABLE_TESTING) 
