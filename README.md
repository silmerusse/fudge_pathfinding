This is a C++ implementation of A* path finding algorithm. The implementation is designed to be generic enough (hopefully) to support different type of maps. The source code has included samples of tiled map pathfinding with single and multiple agents, N-puzzle game, and water jug problem.  

An implementation of JPS(Jump Point Search) for tiled map is provided as an extension to the A* algorithm. In fact, JPS just uses a different way to generate new "neighbors" to be put into the open node list.

Run "cmake" to generate makefiles and "make" to build the source code. 

A simple GUI app is also included under "gui" directory to demostrate agent pathfinding. To build the app, please make sure "SDL" and "JsonCpp" packages are installed.

Tests require "GoogleTest" package.
 
