#ifndef FUDGE_RRA_H_
#define FUDGE_RRA_H_

#include <map>
#include <memory>
#include "astar_search.h"
#include "grid_map.h"

// This is an implementation of RRA* (Reverse Resumable A*) algorithm.
// The instance keeps a series of maps with separated open list and different
// start point. Each search will return the actual cost(g) from the start
// position to end position. After the search, the map and open list is not
// destroyed so that the next search could reuse the data.
namespace fudge {

template <typename CostType = double>
class RRA {
public:
  RRA(const std::vector<CostType> &matrix, int w, int h)
    : matrix_(matrix), w_(w), h_(h) {};

  CostType search(const Coord &start, const Coord &end,
                  CostType heuristic(const Coord&, const Coord&)){
    DEBUG("RRA: Looking for map.");
    if (maps_.find(start) == maps_.end()){
      maps_[start] = std::move(
          std::unique_ptr<GridMap<CostType>>(
              new GridMap<CostType>(w_, h_, matrix_, false)));
      DEBUG("RRA: New map created.");
    }
    int g = maps_.at(start)->node(end)->g_;
    if (g != -1)
      return g;
    else {
      DEBUG("RRA: Start search.");
      std::vector<Coord> &&path = astar_search(
          *(maps_.at(start)), 
          start, end,
          heuristic);
      DEBUG("RRA: End search.");
      return maps_.at(start)->node(path.front())->g_;
    }
  }

private:
  const std::vector<CostType> &matrix_;
  std::map<Coord, std::unique_ptr<GridMap<CostType>>> maps_;
  int w_;
  int h_;
};

}

#endif /* FUDGE_RRA_H_ */
