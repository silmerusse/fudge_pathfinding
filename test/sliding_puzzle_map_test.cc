#include <functional>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "sliding_puzzle_map.h"
#include "astar_search.h"
#include "time_util.h"

// Helper to print and return result.
void print_result(const vector<pair<string, int>> &path,
                  const SlidingPuzzleMap &map){
  std::cout << "--------------------\n";
  ostringstream ss;
  for (auto e : path)
    ss << e.first << ',' << e.second << '\n';
  ss << map.stats_.to_string() << '\n';
  std::cout << ss.str();
}

TEST(SlidingPuzzleMap, search_2x2_not_found) {
  PREPARE_TIMER
  START_TIMER
    SlidingPuzzleMap map(2);
    vector<pair<string, int>> &&path = AStarSearch::search(map,
            make_pair(string("1230"), 0), make_pair(string("0231"), 0),
            std::bind(&SlidingPuzzleMap::manhattan_distance, map,
                placeholders::_1, placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(0, path.size()); // path not found
  ASSERT_EQ(12, map.stats_.nodes_opened); // invalid positions == (N*M)!/2
}

TEST(SlidingPuzzleMap, search_3x3_found) {
  PREPARE_TIMER
  START_TIMER
    SlidingPuzzleMap map(3);
    vector<pair<string, int>> &&path = AStarSearch::search(map,
        make_pair(string("876543210"), 0), make_pair(string("123456780"), 0),
        std::bind(&SlidingPuzzleMap::manhattan_distance, map,
            placeholders::_1, placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(30, path.size());
}

TEST(SlidingPuzzleMap, search_4x4_found) {
  PREPARE_TIMER
  START_TIMER
    SlidingPuzzleMap map(4);

    vector<pair<string, int>> &&path = AStarSearch::search(map,
                          make_pair(string("fedcba9876543210"), 0),
                          make_pair(string("0fdcbea876953214"), 0),
                          std::bind(&SlidingPuzzleMap::manhattan_distance, map,
                              placeholders::_1, placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(6, path.size());
}

