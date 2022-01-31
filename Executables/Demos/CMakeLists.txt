# CMakeLists for Nymph/Executables/Demos
# Author: N.S. Oblath

if( NOT Nymph_AS_SUBMODULE )
    set( Nymph_package_libs
        Nymph
    )
endif()

set( Nymph_SOURCES
    Return.cc
)

set( Nymph_PROGRAMS )
pbuilder_executables( 
    SOURCES ${Nymph_SOURCES}
    TARGETS_VAR Nymph_PROGRAMS
    PROJECT_LIBRARIES ${Nymph_package_libs}
    PUBLIC_EXTERNAL_LIBRARIES ${Nymph_EXE_LIBRARIES}
#        PRIVATE_EXTERNAL_LIBRARIES ${PRIVATE_EXT_LIBS}
)

pbuilder_component_install_and_export( 
    COMPONENT Executables
    EXETARGETS ${Nymph_PROGRAMS}
)
