This is a C++ implementation of A* pathfinding algorithm. The implementation is designed to be generic enough (hopefully) to support different type of maps. The repository includes samples of tiled map pathfinding with single and multiple agents, N-puzzle game, and water jug problem.  

An implementation of JPS(Jump Point Search) for tiled map is provided as an extension to the A* algorithm. In fact, JPS just uses a different way to generate new "neighbors" to be put into the open node list.

Run "cmake" to generate makefiles and then "make" to build the source code. 

A simple GUI app is also included in "gui" directory to demostrate agent pathfinding. To build the app, please make sure "SDL2" package is installed. Click left mouse button to select an agent. Clik right mouse button to set target. Press "B" and click right mouse button to build a new agent and put it on the map.

![Screenshot](gui/screenshot/screenshot0.png?raw=true)

Tests require "GoogleTest" package.

 
