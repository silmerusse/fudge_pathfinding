#include <iostream>
#include "grid_map.h"
#include "astar_search.h"
#include "load_matrix.h"
#include "time_util.h"

int main (int argc, char *argv[]) {
  vector<double> matrix = load_matrix<double>("../data/matrix_10x10_wall.txt");

  PREPARE_TIMER
  START_TIMER
  GridMap<double> map(10, 10, matrix);
  vector<Coord> &&path = AStarSearch::search(map, Coord(7, 0), Coord(4, 1),
                                           GridMap<double>::diagonal_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  for (auto i = path.rbegin(); i != path.rend(); ++i)
    std::cout << map.node(*i)->to_string() << std::endl;
  std::cout << map.to_string() << std::endl;

  return 0;
}
