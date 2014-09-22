#include <iostream>
#include "sliding_puzzle_map.h"
#include "astar_search.h"
#include "time_util.h"

int main (int argc, char *argv[]) {
  PREPARE_TIMER
  START_TIMER
  SlidingPuzzleMap map(3);
  vector<pair<string, int>> &&path = AStarSearch::search(map,
      make_pair(string("876543210"), 0), make_pair(string("123456780"), 0),
      std::bind(&SlidingPuzzleMap::manhattan_distance, map,
          placeholders::_1, placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  for (auto e : path)
    std::cout << e.first << " f:" << e.second << std::endl;
  std::cout << map.to_string() << std::endl;

  return 0;
}
