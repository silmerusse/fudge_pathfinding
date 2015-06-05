file(GLOB SRC_FILES "*.cc")
add_executable(fudge_gui ${SRC_FILES})
include_directories(../sample)
target_link_libraries(fudge_gui SDL2 SDL2 SDL2_image SDL2_ttf jsoncpp)

	
