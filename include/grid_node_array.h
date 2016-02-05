#ifndef FUDGE_GRID_NODE_ARRAY_H_
#define FUDGE_GRID_NODE_ARRAY_H_

#include <string>
#include <sstream>
#include <memory>
#include "grid_node.h"

// This is used to hold grid nodes for quick indexing.

namespace fudge {

template<typename CostType>
class GridNodeArray {
public:
  explicit GridNodeArray(int w, int h):w_(w), h_(h) {
    reset();
  }

  virtual ~GridNodeArray() = default;

public:
  int w_ = 0;
  int h_ = 0;

public:
  void reset() {
    array_.reset(new GridNode<CostType>[h_*w_]);
    for (int i=0; i < h_; i++) {
      for (int j=0; j < w_; j++) {
          node(Coord(j, i))->x(j);
          node(Coord(j, i))->y(i);
      }
    }
  }

  GridNode<CostType> *node(Coord coord) const{
    return &array_.get()[coord.second * w_ + coord.first];
  }

  bool off(Coord coord) const {
    if (coord.first < 0 || coord.first >= w_ ||
        coord.second < 0 || coord.second >= h_)
      return true;
    else
      return false;
  }

private:
  std::unique_ptr<GridNode<CostType>> array_ = nullptr;
};

}

#endif /* FUDGE_GRID_NODE_ARRAY_H_ */
