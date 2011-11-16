FIND_PACKAGE(liblo REQUIRED)

ADD_EXECUTABLE(emokit_osc emokit_osc.cpp)

TARGET_LINK_LIBRARIES(emokit_osc epoc ${LIBLO_LIBRARY})

if (WIN32)
	# link libs needed by liblo:
	target_link_libraries(emokit_osc wsock32 ws2_32)
endif()
