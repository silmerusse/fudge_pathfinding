This is a C++ implementation of A* path finding algorithm. The implementation is designed to be generic enough (hopefully) to support different type of maps. The source code has already included samples using 8-way tiled map and position maps for N-puzzle game and water jug problem. User could derive from them or provide own map class for specific need. 

An implementation of Jump Point Search is also provided as an extension of the A* algorithm. Basically JPS just uses a different way to generate new "neighbors" to be put into the open node list.

All source code are provided as headers. Besides them, there are a few samples and tests included. Simply run "make" in the corresponding directories to build them. Please note, tests require Google Test packages installed. 
