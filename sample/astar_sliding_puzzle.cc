#include <memory>
#include <iostream>
#include "sliding_puzzle_map.h"
#include "astar_search.h"
#include "util/time_util.h"

// There's a puzzle with initial position like this:
// 876
// 543
// 210
// We want to solve the puzzle with the target position as follows:
// 123
// 456
// 780
int main (int argc, char *argv[]) {
  SlidingPuzzleMap map(3);

  PREPARE_TIMER
  START_TIMER

  const std::vector<SlidingPosition> path = fudge::astar_search(
      map, SlidingPosition("876543210"), SlidingPosition("123456780"),
      std::bind(&SlidingPuzzleMap::manhattan_distance, map,
                std::placeholders::_1, std::placeholders::_2));

  END_TIMER
  PRINT_TIME_ELAPSED

  for (auto i = path.rbegin(); i != path.rend() ; ++i)
    std::cout << i->to_string() << " --- " << i->cost_ << std::endl;
  std::cout << map.stats_.to_string() << std::endl;

  return 0;
}
