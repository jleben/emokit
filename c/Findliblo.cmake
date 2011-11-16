# Findliblo.cmake
#
# This CMake module will try to find the 'liblo' library,
# and set the following variables:
#
# LIBLO_INCLUDE_DIR - liblo headers dirs to include
# LIBLO_LIBRARY - liblo library to link to
# LIBLO_FOUND - whether liblo was found successfully

if (LIBLO_INCLUDE_DIR AND LIBLO_LIBRARY)
	set(LIBLO_FOUND TRUE)
else()

	find_path( LIBLO_INCLUDE_DIR lo/lo.h )
	find_library( LIBLO_LIBRARY lo )

	if( LIBLO_INCLUDE_DIR AND LIBLO_LIBRARY )
		set(LIBLO_FOUND TRUE)
	else()
		set(LIBLO_FOUND FALSE)
	endif()

	if(LIBLO_FOUND)
		if (NOT LIBLO_FIND_QUIETLY)
			  message(STATUS "Found liblo:")
			  message(STATUS " * Includes: ${LIBLO_INCLUDE_DIR}")
			  message(STATUS " * Libraries: ${LIBLO_LIBRARY}")
		endif ()
	else()
		if (liblo_FIND_REQUIRED)
			message(FATAL_ERROR "Could not find liblo!")
		endif()
	endif()

	mark_as_advanced(LIBLO_INCLUDE_DIR LIBLO_LIBRARY)

endif()
