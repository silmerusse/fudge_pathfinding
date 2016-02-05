#ifndef FUDGE_MAP_H_
#define FUDGE_MAP_H_

#include <vector>
#include "edge.h"

namespace fudge {

template <typename NodeType, typename CostType>
class Map {
public:
  Map() {};
  virtual ~Map() {};

public:
  // Return cost of the node (g cost).
  virtual CostType current_cost(const NodeType &n) const = 0;

  // Return valid edges to valid neighbors.
  virtual const std::vector<Edge<NodeType, CostType>> 
  edges (const NodeType &n) = 0;

  // Check of node state.
  virtual bool nodes_equal(const NodeType &n0, const NodeType &n1) const = 0;
  virtual bool open_node_available() const = 0;
  virtual bool is_node_unexplored(const NodeType &n) const = 0;
  virtual bool is_node_open(const NodeType &n) const = 0;

  // Change node state. 
  virtual void open_node(const NodeType &n, CostType g, CostType h,
                         const NodeType &p) = 0;
  virtual void reopen_node(const NodeType &n, CostType g, CostType h,
                           const NodeType &p) = 0;
  virtual NodeType take_out_top_node() = 0;
  virtual void increase_node_priority(const NodeType &n, CostType g, CostType h,
                                      const NodeType &p) = 0;

  // Return the result of path found.
  virtual std::vector<NodeType> get_path(const NodeType &n) = 0;

public:
  // Compare cost of double type. Epsilon is considered.
  virtual bool cost_greater(const double c0, const double c1) const {
    return c0 - c1 > 0.00001; // epsilon
  }

  virtual bool cost_less(const double c0, const double c1) const {
    return c0 - c1 < -0.00001; // epsilon
  }

  // Compare cost of float type. Epsilon is considered.
  virtual bool cost_greater(const float c0, const float c1) const {
    return c0 - c1 > 0.00001; // epsilon
  }

  virtual bool cost_less(const float c0, const float c1) const {
    return c0 - c1 < -0.00001; // epsilon
  }

  // Compare cost of integer type. 
  virtual bool cost_greater(const int c0, const int c1) const {
    return c0 - c1 > 0;
  }

  virtual bool cost_less(const int c0, const int c1) const {
    return c0 - c1 < 0;
  }
};

}

#endif /* FUDGE_MAP_H_ */
