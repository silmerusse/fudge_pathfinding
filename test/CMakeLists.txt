file(GLOB SRC_FILES "*.cc")
add_executable(fudge_test ${SRC_FILES})
include_directories(../include ../sample)
target_link_libraries(fudge_test gtest gtest_main pthread)
SET(CMAKE_CXX_FLAGS "-std=c++0x -g -O0")


	
