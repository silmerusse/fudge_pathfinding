#ifndef FUDGE_GRID_NODE_H_
#define FUDGE_GRID_NODE_H_

#include <algorithm>
#include <sstream>
#include <string>
#include "node_state.h"
#include "map.h"

namespace fudge {

using Coord=std::pair<int,int>;

// This represents a node in a tiled grid map that holds cost, state,
// coordinate, and a pointer to its parent node, .
template<typename CostType>
class GridNode {
public:
  GridNode(int x, int y, CostType f) : c_(x,y), f_(f) {};
  GridNode(Coord c, CostType f) : c_(c), f_(f) {};
  GridNode() {};

public:
  Coord c_ = {-1, -1};
  CostType g_ = -1; // cost from start to current
  CostType f_ = -1; // total cost (current + estimated)
  NodeState state_ = NodeState::unexplored;

public:
  GridNode *parent_ = nullptr;

public:
  static constexpr bool less_priority(const GridNode *a, const GridNode *b) {
    return a->f_ - b->f_ > 0.00001;
  }

  static constexpr double get_priority(const GridNode *a) {
    return a->f_;
  }

  static void set_priority(GridNode *&a, CostType v) {
    a->f_ =v;
  }

public:
  constexpr int x() const {
    return c_.first;
  }

  constexpr int y() const {
    return c_.second;
  }

  void x(int x1) {
    c_.first = x1;
  }

  void y(int y1) {
    c_.second = y1;
  }

public:
  friend std::ostream& operator <<(std::ostream &out,
                                   GridNode<CostType> const &n) {
    out << n.f_;
    return out;
  }

  friend std::ostream& operator <<(std::ostream &out,
                                   GridNode<CostType>* const &n) {
    out << n->f_;
    return out;
  }

  const std::string to_string() const {
    std::ostringstream ss;
    ss << "(" << x() << "," << y() << ")"
       << "<-(" << (parent_ != nullptr ? parent_->x() : -1)
       << "," << (parent_ != nullptr ? parent_->y() : -1) << ")"
       << " g:" << g_
       << " f:" << f_;

    return ss.str();
  }

};

}
#endif /* FUDGE_GRID_NODE_H_ */
