cmake_minimum_required (VERSION 2.6)

SET(CMAKE_CXX_FLAGS "-std=c++0x -g -O0")

file(GLOB SRC_FILES "*.cc")
file(GLOB OBJECT_SRC_FILES "object/*.cc")

add_executable(fudge_gui ${SRC_FILES} ${OBJECT_SRC_FILES})
include_directories(include ../include)
target_link_libraries(fudge_gui SDL2 SDL2_image SDL2_ttf)
SET(CMAKE_CXX_FLAGS "-std=c++0x")


	
