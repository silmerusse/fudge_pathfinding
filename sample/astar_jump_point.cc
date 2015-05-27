#include <iostream>
#include "jump_point_map.h"
#include "astar_search.h"
#include "load_matrix.h"
#include "time_util.h"

// Find path from start position to target position using Jump Point algorithm.
int main (int argc, char *argv[]) {
  vector<double> matrix = load_matrix<double>("../data/matrix_10x10_wall.txt");

  PREPARE_TIMER
  START_TIMER
  JumpPointMap<double> map(10, 10, matrix);
  Coord start = Coord(7, 0);
  Coord goal = Coord(4, 1);
  map.goal_ = goal; // This is needed for checking Jump Point.
  vector<Coord> &&path = AStarSearch::search(
      map, start, goal,
      GridMap<double>::diagonal_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  for (auto e : path)
    std::cout << map.node(e)->to_string() << std::endl;
  std::cout << map.to_string() << std::endl;

  return 0;
}
