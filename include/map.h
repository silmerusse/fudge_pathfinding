#ifndef MAP_H_
#define MAP_H_
#include <vector>
#include "edge.h"

using namespace std;

template <typename NodeType, typename CostType>
class Map {
public:
  Map() {};
  virtual ~Map() {};

public:
  // Return g-score of the node.
  virtual CostType current_cost(const NodeType &n) const = 0;

  // Return valid edges that connect to nodes could be explored.
  virtual const vector<Edge<NodeType, CostType> > edges (const NodeType &n) = 0;

  // Checks
  virtual bool nodes_equal(const NodeType &n0, const NodeType &n1) const = 0;
  virtual bool opon_node_available() = 0;
  virtual bool node_unexplored(const NodeType &n) const = 0;
  virtual bool node_open(const NodeType &n) const = 0;

  // Operations
  virtual void open_node(NodeType &n, CostType g, CostType h,
                         const NodeType &p) = 0;
  virtual void reopen_node(NodeType &n, CostType g, CostType h,
                           const NodeType &p) = 0;
  virtual NodeType close_front_open_node() = 0;
  virtual void increase_node_priority(NodeType &n, CostType g, CostType h,
                                      const NodeType &p) = 0;

  // Do some initialization work before the search get started.
  virtual void initialize(const NodeType &start, const NodeType &goal) = 0;

  // Return the result of path found.
  virtual vector<NodeType> get_path(const NodeType &n) = 0;

public:
  // Compare two cost variables of double type. Epsilon is considered.
  virtual bool cost_greater(const double c0, const double c1) const {
    return c0 - c1 > 0.00001; // epsilon
  }

  // Compare two cost variables of float type. Epsilon is considered.
  virtual bool cost_greater(const float c0, const float c1) const {
    return c0 - c1 > 0.00001; // epsilon
  }

  virtual bool cost_greater(const int c0, const int c1) const {
    return c0 - c1 > 0;
  }
};

#endif /* MAP_H_ */
