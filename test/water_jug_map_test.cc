#include <functional>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "water_jug_map.h"
#include "astar_search.h"
#include "time_util.h"

// Helper to print and return result.
void print_result(const std::vector<WaterJugPosition> &path,
                  const WaterJugMap &map){
  std::cout << "--------------------\n";
  std::ostringstream ss;
  for (auto i = path.rbegin(); i != path.rend(); ++i)
    ss << i->to_string() << " --- " << i->cost_ << std::endl;
  ss << map.stats_.to_string() << std::endl;
  std::cout << ss.str();
}

TEST(WaterJugMap, search) {
  PREPARE_TIMER
  START_TIMER
    WaterJugMap map({21, 15, 8, 5});
    std::vector<WaterJugPosition> &&path = AStarSearch::search(
        map, WaterJugPosition({21, 0, 0, 0}),
        WaterJugPosition({7, 7, 7, 0}),
        std::bind(&WaterJugMap::heuristic, map,
            std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(12, path.size());
  ASSERT_EQ(421, map.stats_.nodes_opened);
}
