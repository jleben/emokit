##########################################
##              libepoc                ###
##########################################

if (BUILD_SHARED_LIBS)
    message (STATUS "Building libepoc as shared library.")
else()
    message (STATUS "Building libepoc as static library.")
endif()

find_package(Mcrypt REQUIRED)
list (APPEND LIBEPOC_LIBS ${Mcrypt_LIBS})
include_directories(${Mcrypt_INCLUDE_DIR})

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
    include_directories(${WDDK_INCLUDE_DIRS})

    list(APPEND LIBEPOC_LIBS hid setupapi)
elseif(UNIX)
    find_package(libusb-1.0 REQUIRED)
    if(LIBUSB_1_FOUND)
        include_directories(${LIBUSB_1_INCLUDE_DIRS})
        list(APPEND LIBEPOC_LIBS ${LIBUSB_1_LIBRARIES})
    endif(LIBUSB_1_FOUND)
endif()

set(LIBEPOC_HEADERS ${CMAKE_SOURCE_DIR}/include/libepoc.h)

set(LIBEPOC_SRC epoc.c)
if (WIN32)
    list (APPEND LIBEPOC_SRC epoc_win32.c)
else()
    list (APPEND LIBEPOC_SRC epoc_libusb.c)
endif()

add_library (epoc ${LIBEPOC_SRC})
target_link_libraries (epoc ${LIBEPOC_LIBS})

install (TARGETS epoc
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES
  ${LIBEPOC_HEADERS}
  DESTINATION include/libepoc
)
