CXX := g++
CCFLAGS := -O3 -I../../include/util -I../include -std=c++0x
TARGETS := astar_grid_map astar_jump_point astar_sliding_puzzle

all: $(TARGETS)

astar_grid_map: astar_grid_map.cc
	$(CXX) $(CCFLAGS) $< -o $@
	
astar_jump_point: astar_jump_point.cc
	$(CXX) $(CCFLAGS) $< -o $@
  
astar_sliding_puzzle: astar_sliding_puzzle.cc
	$(CXX) $(CCFLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(TARGETS)
