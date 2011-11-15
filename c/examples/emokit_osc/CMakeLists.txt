
ADD_EXECUTABLE(emokit_osc emokit_osc.cpp)
ADD_DEPENDENCIES(emokit_osc epoc)
TARGET_LINK_LIBRARIES(emokit_osc lo epoc)
