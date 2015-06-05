file(GLOB SRC_FILES "*.cc")
add_executable(fudge_test ${SRC_FILES})
include_directories(../sample)
target_link_libraries(fudge_test gtest gtest_main pthread)

	
