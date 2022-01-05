# Nymph.cmake
# Macros for building a project using Nymph
# Author: N. Oblath

# Main directory for Nymph
set (NYMPH_DIR ${CMAKE_CURRENT_LIST_DIR}/..)


##########
# Macros #
##########

# This should be called immediately after setting the project name
macro (nymph_process_options)

    # Single-threading option
	if (Nymph_SINGLETHREADED)
	    add_definitions(-DSINGLETHREADED)
	else (Nymph_SINGLETHREADED)
	    remove_definitions(-DSINGLETHREADED)
	endif(Nymph_SINGLETHREADED)

    # Optional compiler flags
	if (${CMAKE_BUILD_TYPE} MATCHES RELEASE|RelWithDebInfo|MinSizeRel/i)
	    set(FLAG_WARN_AS_ERROR TRUE)
	endif ()
	if (FLAG_WARN_AS_ERROR)
	    message(STATUS "warnings as errors")
	    set (FLAG_WARNINGS "-Werror ${FLAG_WARNINGS}")
	else (FLAG_WARN_AS_ERROR)
	    STRING(REGEX REPLACE "-Werror " "_" FLAG_WARNINGS "${FLAG_WARNINGS}")
	endif(FLAG_WARN_AS_ERROR)
	if (${FLAG_WARNING_LEVEL} MATCHES NONE)
	    STRING(REGEX REPLACE "-Wall |-Wextra " "" FLAG_WARNINGS "${FLAG_WARNINGS}")
	    set (FLAG_WARNINGS "${FLAG_WARNINGS} -w")
	endif (${FLAG_WARNING_LEVEL} MATCHES NONE)
	if (${FLAG_WARNING_LEVEL} MATCHES NORMAL)
	    STRING(REGEX REPLACE "-Wall |-Wextra |-w" "" FLAG_WARNINGS "${FLAG_WARNINGS}")
	endif (${FLAG_WARNING_LEVEL} MATCHES NORMAL)
	if (${FLAG_WARNING_LEVEL} MATCHES MORE)
	    STRING(REGEX REPLACE "-Wextra |-w" "" FLAG_WARNINGS "${FLAG_WARNINGS}")
	    set (FLAG_WARNINGS "-Wall ${FLAG_WARNINGS}")
	endif (${FLAG_WARNING_LEVEL} MATCHES MORE)
	if (${FLAG_WARNING_LEVEL} MATCHES MOST)
	    STRING(REGEX REPLACE "-w" "" FLAG_WARNINGS "${FLAG_WARNINGS}")
	    set (FLAG_WARNINGS "-Wall -Wextra ${FLAG_WARNINGS}")
	endif (${FLAG_WARNING_LEVEL} MATCHES MOST)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG_WARNINGS}")
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG_WARNINGS}")

    if( UNIX AND NOT APPLE )
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")
    endif( UNIX AND NOT APPLE )

endmacro ()

macro (nymph_build_executables)
	add_subdirectory (${NYMPH_DIR}/Executables/Main)
endmacro ()

