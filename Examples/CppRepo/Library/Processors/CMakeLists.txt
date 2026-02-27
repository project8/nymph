# CMakeLists for nymph/Examples/CppRepo/Library/Processors
# Author: N.S. Oblath

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CppRepo_LIBS 
    CppRepoData
)

set( CppRepoProcs_HEADERFILES
    Adder.hh
)

set( CppRepoProcs_SOURCEFILES
    Adder.cc
)


##################################################

pbuilder_library( 
    TARGET CppRepoProcs 
    SOURCES ${CppRepoProcs_SOURCEFILES}
    PUBLIC_EXTERNAL_LIBRARIES ${PUBLIC_EXT_LIBS}
    PROJECT_LIBRARIES ${CppRepo_LIBS}
)

pbuilder_component_install_and_export( 
    COMPONENT Library
    LIBTARGETS CppRepoProcs
)

pbuilder_install_headers( ${CppRepoProcs_HEADERFILES} )
