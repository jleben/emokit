if (LIBLO_INCLUDE_DIR AND LIBLO_LIBRARY)
	set(LIBLO_FOUND TRUE)
else()

	find_path( LIBLO_INCLUDE_DIR
		NAMES lo/lo.h
	)
	find_library( LIBLO_LIBRARY
		NAMES liblo
	)

	if( LIBLO_INCLUDE_DIR AND LIBLO_LIBRARY )
		set(LIBLO_FOUND TRUE)
	endif()

	if(LIBLO_FOUND)
		if (NOT LIBLO_FIND_QUIETLY)
			  message(STATUS "Found liblo:")
			  message(STATUS " - Includes: ${LIBLO_INCLUDE_DIR}")
			  message(STATUS " - Libraries: ${LIBLO_LIBRARY}")
		endif ()
	else()
		if (LIBLO_FIND_REQUIRED)
			message(FATAL_ERROR "Could not find liblo!")
		endif()
	endif()

	mark_as_advanced(LIBLO_INCLUDE_DIRS LIBLO_LIBRARIES)

endif()
