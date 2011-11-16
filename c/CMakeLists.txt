cmake_minimum_required (VERSION 2.8)
PROJECT(EMOKIT)

option(BUILD_SHARED_LIBS "Whether libepoc shall be built shared" true)
if (BUILD_SHARED_LIBS)
    message (STATUS "Building libepoc as shared library.")
else()
    message (STATUS "Building libepoc as static library.")
endif()

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

FIND_PACKAGE(Mcrypt REQUIRED)
INCLUDE_DIRECTORIES(${Mcrypt_INCLUDE_DIR})
LIST(APPEND LIBEPOC_REQUIRED_LIBS ${Mcrypt_LIBS})

if (WIN32)
    set(WDK_PATH CACHE PATH "Path to Windows Driver Kit installation")
    if(WDK_PATH)
        set(WDDK_INCLUDE_DIRS ${WDK_PATH}/inc/api ${WDK_PATH}/inc/crt ${WDK_PATH}/inc)
        link_directories(${WDK_PATH}/lib/wxp/i386)
    elseif (NOT WDDK_INCLUDE_DIRS)
        find_path(WDDK_INCLUDE_DIRS hidsdi.h)
        if (NOT WDDK_INCLUDE_DIRS)
            message( WARNING
                "No path to Windows Driver Kit has been set, "
                "and Driver Device Kit headers do not seem to be available. "
                "Please set WDK_PATH to a top WDK directory, "
                "or add a directory with DDK includes to CMAKE_INCLUDE_PATH.")
             set(WDDK_INCLUDE_DIRS "")
         endif()
    endif()
    mark_as_advanced(WDDK_INCLUDE_DIRS)

    list(APPEND LIBEPOC_REQUIRED_LIBS hid setupapi)
elseif(UNIX)
    find_package(libusb-1.0 REQUIRED)
    if(LIBUSB_1_FOUND)
        include_directories(${LIBUSB_1_INCLUDE_DIRS})
        list(APPEND LIBEPOC_REQUIRED_LIBS ${LIBUSB_1_LIBRARIES})
    endif(LIBUSB_1_FOUND)
endif()

include_directories(${EMOKIT_SOURCE_DIR}/include)
set(LIBEPOC_HEADERS ${EMOKIT_SOURCE_DIR}/include/libepoc.h)

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

