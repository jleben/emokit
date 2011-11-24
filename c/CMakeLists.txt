cmake_minimum_required (VERSION 2.8)
PROJECT(Emokit)

option ( EMOKIT_USE_STATIC_LIBS
    "Whether to link all the dependencies statically, as far as this is possible"
    FALSE
)

option ( BUILD_SHARED_LIBS
    "Whether the libraries built by this project will be shared or static"
    TRUE
)

if(CMAKE_COMPILER_IS_GNUCC)
    if(EMOKIT_USE_STATIC_LIBS)
        message(STATUS "Using static GNU libraries.")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    else()
        message(STATUS "Using shared GNU libraries.")
    endif()
endif()

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples/epocd)
ADD_SUBDIRECTORY(examples/emokit_osc)

##########################################
##       PKGCONFIG file                ###
##########################################

IF   (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libepoc.pc.in")
    MESSAGE(STATUS "configured ${CMAKE_CURRENT_SOURCE_DIR}/libepoc.pc.in --> ${CMAKE_CURRENT_BINARY_DIR}/libepoc.pc")
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/libepoc.pc.in 
      ${CMAKE_CURRENT_BINARY_DIR}/libepoc.pc
      @ONLY )

	INSTALL(FILES
		${CMAKE_CURRENT_BINARY_DIR}/libepoc.pc
		DESTINATION lib/pkgconfig
	)
ENDIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libepoc.pc.in")

