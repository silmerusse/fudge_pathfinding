#include <iostream>
#include "jump_point_map.h"
#include "astar_search.h"
#include "load_matrix.h"
#include "util/time_util.h"

// Search shortest path using Jump Point Search algorithm.
int main (int argc, char *argv[]) {
  std::vector<double> matrix = fudge::load_matrix<double>(
      "../data/matrix_10x10_wall.txt");

  fudge::JumpPointMap<double> map(10, 10, matrix);
  fudge::Coord start(7, 0);
  fudge::Coord goal(4, 1);
  map.goal_ = goal; // This is needed for checking Jump Point.

  PREPARE_TIMER
  START_TIMER

  const std::vector<fudge::Coord> path = fudge::astar_search(
      map, start, goal,
      fudge::GridMap<double>::diagonal_distance);

  END_TIMER
  PRINT_TIME_ELAPSED

  for (const auto &e : path)
    std::cout << map.node(e)->to_string() << std::endl;
  std::cout << map.to_string() << std::endl;

  return 0;
}
