#include <gtest/gtest.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>
#include "astar_search.h"
#include "util/time_util.h"
#include "torches_puzzle.h"

// Helper to print and return result.
void print_result(const std::vector<TorchesPosition> &path,
                  const TorchesPuzzle &map){
  std::cout << "--------------------\n";
  std::ostringstream ss;
  for (auto i = path.rbegin(); i != path.rend(); ++i)
    ss << i->to_string() << " --- " << i->cost_ << std::endl;
  ss << map.stats_.to_string() << std::endl;
  std::cout << ss.str();
}

TEST(TorchesPuzzle, search) {
  PREPARE_TIMER
  START_TIMER
  TorchesPuzzle map;

  // Set initial state.
  TorchesPosition start({
    {1, 6},
    {2, 3},
    {3, 4},
    {4, 6},
    {5, 3},
    {6, 0},
    {0, 1}});
  start.pos_[0].on_ = true;
  start.pos_[1].on_ = true;

  // Set target state.
  TorchesPosition end = start;
  for (auto &t : end.pos_)
    t.on_ = true;

  std::vector<TorchesPosition> &&path = AStarSearch::search(
      map, start, end,
      std::bind(&TorchesPuzzle::heuristic, map,
                std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(4, path.size());
  ASSERT_EQ(28, map.stats_.nodes_opened);
}
