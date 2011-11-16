##########################################
##              libepoc                ###
##########################################

# Create a library called "libepoc"
IF(WIN32)
    include_directories(${WDDK_INCLUDE_DIRS})
  ADD_LIBRARY (epoc epoc.c epoc_win32.c)
ELSE()
  ADD_LIBRARY (epoc epoc.c epoc_libusb.c)
ENDIF()

target_link_libraries (epoc ${LIBEPOC_REQUIRED_LIBS})

INSTALL (TARGETS epoc
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

INSTALL(FILES
  ${LIBEPOC_HEADERS}
  DESTINATION include/libepoc
  )
