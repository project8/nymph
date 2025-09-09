# CMakeLists for nymph/Examples/CppRepo/Library/Data
# Author: N.S. Oblath

include_directories( BEFORE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CppRepoData_HEADERFILES
    DoubleData.hh
    IntData.hh
)

set( CppRepoData_SOURCEFILES
    DoubleData.cc
    IntData.cc
)


##################################################

pbuilder_library( 
    TARGET CppRepoData 
    SOURCES ${CppRepoData_SOURCEFILES}
    PUBLIC_EXTERNAL_LIBRARIES ${PUBLIC_EXT_LIBS}
)

pbuilder_component_install_and_export( 
    COMPONENT Library
    LIBTARGETS CppRepoData
)

pbuilder_install_headers( ${CppRepoData_HEADERFILES} )
