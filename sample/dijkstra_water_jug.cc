#include <memory>
#include <iostream>
#include "water_jug_map.h"
#include "astar_search.h"
#include "util/time_util.h"

// There are 4 jugs with maximal capacity of 21, 15, 8, and 5 liter each.
// We could pour water from one jug to another jug to fill the other jug
// or empty the current jug. In any case, no water should spill over.
// Initially only the first jug has 21 liter of water. Other jugs are empty.
// We want to reach the target state that 4 jugs have 7, 7, 7, and 0 liter of
// water each.
int main (int argc, char *argv[]) {
  PREPARE_TIMER
  START_TIMER
  WaterJugMap map(std::vector<int>{21, 15, 8, 5});

  std::vector<WaterJugPosition> &&path = AStarSearch::search(
      map,
      WaterJugPosition({21, 0, 0, 0}),
      WaterJugPosition({7, 7, 7, 0}),
      std::bind(&WaterJugMap::heuristic, map,
                std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  for (auto i = path.rbegin(); i != path.rend(); ++i)
    std::cout << i->to_string() << " --- " << i->cost_ << std::endl;
  std::cout << map.stats_.to_string() << std::endl;

  return 0;
}
