# CMakeLists for Nymph/Executables/Main
# Author: N. Oblath

if( Nymph_ENABLE_EXECUTABLES )

    get_property( all_libs GLOBAL PROPERTY ${PROJECT_NAME}_LIBRARIES )
    message(STATUS "Nymph exectuables library dependencies: ${all_libs}" )
    
    set( programs
        ConfigCheck
    )
    
    if( Nymph_BUILD_NYMPH_EXE )
        set( programs
            Nymph
        )
    endif( Nymph_BUILD_NYMPH_EXE )
    
    pbuilder_executables( programs all_libs )
    
endif( Nymph_ENABLE_EXECUTABLES )
