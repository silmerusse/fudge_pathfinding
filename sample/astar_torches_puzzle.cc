#include <memory>
#include <iostream>
#include "astar_search.h"
#include "util/time_util.h"
#include "torches_puzzle.h"

// There are 7 torches. Each true controls another two torches. 
// Each turn the state of a torch and torches under its control are toggled. 
// Assume at the beginning there are two torches that have been turned on and 
// others are turned off. Try to find the least steps to turn on all torches.
int main (int argc, char *argv[]) {
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

  for (auto i = path.rbegin(); i != path.rend(); ++i)
    std::cout << i->to_string() << " --- " << i->cost_ << std::endl;
  std::cout << map.stats_.to_string() << std::endl;

  return 0;
}
