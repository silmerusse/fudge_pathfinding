#include <functional>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "water_jug_map.h"
#include "astar_search.h"
#include "time_util.h"

// Helper to print and return result.
void print_result(const vector<WaterJugNode> &path,
                  const WaterJugMap &map){
  std::cout << "--------------------\n";
  ostringstream ss;
  for (auto i = path.rbegin(); i != path.rend(); ++i)
    ss << i->to_string() << " --- " << i->cost_ << std::endl;
  ss << map.stats_.to_string() << std::endl;
  std::cout << ss.str();
}

TEST(WaterJugMap, search) {
  PREPARE_TIMER
  START_TIMER
    WaterJugMap map(std::vector<int>{21, 15, 8, 5});
    vector<WaterJugNode> &&path = AStarSearch::search(map,
            WaterJugNode(std::vector<int>{21, 0, 0, 0}),
            WaterJugNode(std::vector<int>{7, 7, 7, 0}),
            std::bind(&WaterJugMap::heuristic, map,
                placeholders::_1, placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(12, path.size()); // path not found
  ASSERT_EQ(419, map.stats_.nodes_opened); // invalid positions == (N*M)!/2
}
