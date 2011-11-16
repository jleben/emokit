##################################################
#                epoc daemon                   #
##################################################

add_executable (epocd epocd.c)
target_link_libraries (epocd epoc)

set_target_properties(epocd PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
    BUILD_WITH_INSTALL_RPATH TRUE
)

install (TARGETS epocd
    RUNTIME DESTINATION bin
)
