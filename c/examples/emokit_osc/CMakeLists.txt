find_package(liblo REQUIRED)
include_directories(${LIBLO_INCLUDE_DIR})

add_executable(emokit_osc emokit_osc.cpp)
target_link_libraries(emokit_osc epoc ${LIBLO_LIBRARY})
if (WIN32)
    # link libs needed by liblo:
    target_link_libraries(emokit_osc wsock32 ws2_32)
endif()

install (TARGETS emokit_osc
    RUNTIME DESTINATION bin
)
