#include <iostream>
#include "jump_point_map.h"
#include "astar_search.h"
#include "load_matrix.h"
#include "time_util.h"

int main (int argc, char *argv[]) {
  vector<double> matrix = load_matrix<double>("../data/matrix_10x10_wall.txt");

  PREPARE_TIMER
  START_TIMER
  JumpPointMap<double> map(10, 10, matrix);
  vector<Coord> &&path = AStarSearch::search(map, Coord(7, 0), Coord(4, 1),
                                           GridMap<double>::diagonal_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  for (auto e : path)
    std::cout << map.node(e)->to_string() << std::endl;
  std::cout << map.to_string() << std::endl;

  return 0;
}
