cmake_minimum_required (VERSION 2.6)
project (fudge_pathfinding)

set (fudge_pathfinding_VERSION_MAJOR 1)
set (fudge_pathfinding_VERSION_MINOR 0)

SET(CMAKE_CXX_FLAGS "-std=c++0x") 

include_directories(
  "include"
  "include/util"
)


add_subdirectory(sample)
add_subdirectory(test)
add_subdirectory(gui)
	
