cmake_minimum_required (VERSION 2.8)
PROJECT(Emokit)

option(BUILD_SHARED_LIBS "Whether libepoc shall be built shared" true)
if (BUILD_SHARED_LIBS)
    message (STATUS "Building libepoc as shared library.")
else()
    message (STATUS "Building libepoc as static library.")
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

