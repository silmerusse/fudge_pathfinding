#include <functional>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "sliding_puzzle_map.h"
#include "astar_search.h"
#include "util/time_util.h"

// Helper to print and return result.
void print_result(const std::vector<SlidingPosition> &path,
                  const SlidingPuzzleMap &map){
  std::cout << "--------------------\n";
  std::ostringstream ss;
  for (auto i = path.rbegin(); i != path.rend(); ++i)
    ss << i->to_string() << " --- " << i->cost_ << std::endl;
  ss << map.stats_.to_string() << std::endl;
  std::cout << ss.str();
}

// Test if the puzzle is not solvable.
TEST(SlidingPuzzleMap, search_2x2_not_found) {
  PREPARE_TIMER
  START_TIMER
  SlidingPuzzleMap map(2);
  const std::vector<SlidingPosition> path = fudge::astar_search(map,
      SlidingPosition("1230"), SlidingPosition("0231"),
      std::bind(&SlidingPuzzleMap::manhattan_distance, map,
          std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(0, path.size()); // path not found
  ASSERT_EQ(12, map.stats_.nodes_opened); // invalid positions == (N*M)!/2
}

// Test a 3x3 puzzle.
TEST(SlidingPuzzleMap, search_3x3_found) {
  PREPARE_TIMER
  START_TIMER
    SlidingPuzzleMap map(3);
  const std::vector<SlidingPosition> path = fudge::astar_search(map,
        SlidingPosition("876543210"), SlidingPosition("123456780"),
        std::bind(&SlidingPuzzleMap::manhattan_distance, map,
            std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(31, path.size());
}

// Test a 4x4 puzzle.
TEST(SlidingPuzzleMap, search_4x4_found) {
  PREPARE_TIMER
  START_TIMER
    SlidingPuzzleMap map(4);

  const std::vector<SlidingPosition> path = fudge::astar_search(map,
                          SlidingPosition("fedcba9876543210"),
                          SlidingPosition("0fdcbea876953214"),
                          std::bind(&SlidingPuzzleMap::manhattan_distance, map,
                              std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(7, path.size());
}

