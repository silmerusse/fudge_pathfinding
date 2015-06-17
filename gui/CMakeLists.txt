file(GLOB SRC_FILES "*.cc")
file(GLOB OBJECTS_SRC_FILES "objects/*.cc")
file(GLOB ENGINE_SRC_FILES "engine/*.cc")
add_executable(fudge_gui ${SRC_FILES} ${OBJECTS_SRC_FILES} ${ENGINE_SRC_FILES})
include_directories(. ../sample objects engine)
target_link_libraries(fudge_gui SDL2 SDL2 SDL2_image)

	
