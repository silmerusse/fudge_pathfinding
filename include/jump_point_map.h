#ifndef FUDGE_JUMP_POINT_MAP_H_
#define FUDGE_JUMP_POINT_MAP_H_

#include <algorithm>
#include "util/log.h"
#include "node_state.h"
#include "grid_map.h"
#include "search_stats.h"

// This implements a map used for Jump Point Search algorithm.
// JPS could be based on A* so that we only need to provide a special edges()
// method to return jump points collected. Additionally we have to utilize
// initialize() call to mark the goal node on the map so that we could make it a
// jump point.

namespace fudge {

using NodeType = Coord;

#define INVALID_NODE Coord(-1, -1)

template <typename CostType>
class JumpPointMap : public GridMap<CostType> {
public:
  JumpPointMap(int w, int h, std::vector<CostType> &matrix)
      : GridMap<CostType>(w, h, matrix) {};
  virtual ~JumpPointMap() = default;

public:
  // Override to return edges with destination nodes that are jump points.
  const std::vector<Edge<NodeType, CostType>> 
  edges(const NodeType &n) override{
    std::vector <Edge<NodeType, CostType>> es;

    // Search and add jump points at all directions for start node.
    // For others, only search at necessary directions.
    if (this->node(n)->parent_->c_ == n) {
      const std::vector<Coord> &&coords = this->coord_8_neighbors(n);
      for (auto c : coords) {
        push_jump_point(es, c.first, c.second, n);
      }
    } else {
      // Normalize
      int dx = (x(n) - this->node(n)->parent_->x())
          / std::max(abs(x(n) - this->node(n)->parent_->x()), 1);
      int dy = (y(n) - this->node(n)->parent_->y())
          / std::max(abs(y(n) - this->node(n)->parent_->y()), 1);

      if (dx != 0 && dy != 0){ // diagonal move
        //   - + +          x: obstacle
        //   x n +          +: nature neighbor
        //   p x -          -: forced neighbor
        push_jump_point(es, x(n) + dx, y(n), n); // nature straight
        push_jump_point(es, x(n), y(n) + dy, n); // nature straight
        push_jump_point(es, x(n) + dx, y(n) + dy, n); // nature diagonal

        if (!is_passable(x(n), y(n) - dy))
          push_jump_point(es, x(n) + dx, y(n) - dy, n); // forced diagonal

        if (!is_passable(x(n) - dx, y(n)))
          push_jump_point(es, x(n) - dx, y(n) + dy, n); // forced diagonal

      } else { // straight move
        if (dx != 0) {
          //   x x -          x: obstacle
          //   p n +          +: nature neighbor
          //   x x -          -: forced neighbor
          push_jump_point(es, x(n) + dx, y(n), n); // nature straight

          if (!is_passable(x(n), y(n) + 1))
            push_jump_point(es, x(n) + dx, y(n) + 1, n); // forced diagonal

          if (!is_passable(x(n), y(n) - 1))
            push_jump_point(es, x(n) + dx , y(n) - 1, n); // forced diagonal
        } else {
          //   x p x          x: obstacle
          //   x n x          +: nature neighbor
          //   - + -          -: forced neighbor
          push_jump_point(es, x(n), y(n) + dy, n); // nature straight

          if (!is_passable(x(n) + 1, y(n)))
            push_jump_point(es, x(n) + 1, y(n) + dy, n); // forced diagonal

          if (!is_passable(x(n) - 1, y(n)))
            push_jump_point(es, x(n) - 1, y(n) + dy, n); // force diagonal
        }
      }
    }
    return es;
  }

protected:
  // Search recursively to find a jump point.
  NodeType find_jump_point(int x, int y, const NodeType &p) {
    if (!is_passable(x, y))
      return INVALID_NODE;

    NodeType n =Coord(x, y);

    int dx = x - this->x(p);
    int dy = y - this->y(p);

    if (this->nodes_equal(n, goal_))
      return n;

    // Self jump point test (if there's any forced neighbor)
    if (dx != 0 && dy != 0) {
      //   - + +          x: obstacle
      //   x n +          +: nature neighbor
      //   p x -          -: forced neighbor
      if ((is_passable(x - dx, y + dy) && !is_passable(x - dx, y))
           || (is_passable(x + dx, y - dy) && !is_passable(x , y - dy)))
        return n;
    } else {
      if (dx != 0) {
        //   x x -          x: obstacle
        //   p n +          +: nature neighbor
        //   x x -          -: forced neighbor
        if ((is_passable(x + dx, y + 1) && !is_passable(x, y + 1))
             || (is_passable(x + dx, y - 1) && !is_passable(x, y - 1)))
          return n;
      } else {
        //   x p x          x: obstacle
        //   x n x          +: nature neighbor
        //   - + -          -: forced neighbor
        if ((is_passable(x + 1, y + dy) && !is_passable(x + 1, y))
             || (is_passable(x - 1, y + dy) && !is_passable(x - 1, y)))
          return n;
      }
    }

    // Diagonal move long distance straight direction jump point test
    if (dx != 0 && dy != 0) { // diagonal move
      if ((find_jump_point(x + dx, y, n) != INVALID_NODE)
        || (find_jump_point(x, y + dy, n) != INVALID_NODE))
        return n;
    }

    // Self is not jump point. Search others along the way.
    return find_jump_point(x + dx, y + dy, n);
  }


protected:
  // Determine if a node is passable. Note we ignore the weight of the node
  // because Jump Point algorithm does not work on weighted map.
  bool is_passable(int x, int y) {
    return this->vertex_matrix_.is_passable(Coord(x, y), 1);
  }

  // Helper to push a jump point found to the container.
  void push_jump_point(std::vector<Edge<NodeType, CostType>> &es, int x, int y,
                       const NodeType &n) {
    NodeType jp = find_jump_point(x, y, n);
    if (jp != INVALID_NODE)
      es.push_back(Edge<NodeType, CostType>(
          n, jp, GridMap<CostType>::diagonal_distance(n, jp)));
  }

public:
  NodeType goal_;

protected:
  static int x(const NodeType &n) {
    return GridMap<CostType>::x(n);
  }

  static int y(const NodeType &n) {
    return GridMap<CostType>::y(n);
  }
};

}

#endif /* FUDGE_JUMP_POINT_MAP_H_ */
