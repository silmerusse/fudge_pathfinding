#include <iostream>
#include "grid_map.h"
#include "astar_search.h"
#include "load_matrix.h"
#include "util/time_util.h"

// Search shortest path using A* algorithm.
int main (int argc, char *argv[]) {
  std::vector<double> matrix = fudge::load_matrix<double>(
      "../data/matrix_10x10_wall.txt");
  fudge::GridMap<double> map(10, 10, matrix);

  PREPARE_TIMER
  START_TIMER

  const std::vector<std::pair<int,int>> path = fudge::astar_search(
      map, fudge::Coord(7, 0), fudge::Coord(4, 1),
      fudge::GridMap<double>::diagonal_distance);

  END_TIMER
  PRINT_TIME_ELAPSED

  for (auto i = path.rbegin(); i != path.rend(); ++i)
    std::cout << map.node(*i)->to_string() << std::endl;
  std::cout << map.to_string() << std::endl;

  return 0;
}
