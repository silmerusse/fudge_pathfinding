#ifndef GRID_MAP_H_
#define GRID_MAP_H_

#include <algorithm>

#define LOG_LEVEL 2
#include "log.h"

#include "node_state.h"
#include "map.h"
#include "vertex_matrix.h"
#include "grid_node_array.h"
#include "binary_heap.h"
#include "hot_queue.h"
#include "search_stats.h"

using NodeType = Coord;

// This implements a square tile based grid map.
// It takes a template argument to support different cost type like int and
// double.
// To create a map, a matrix that holds vertex data should be passed.
// Note a map should not be reused after a search. We need to clean all data
// structures anyway. Let's just create a new map.
template <typename CostType = double>
class GridMap : public Map<Coord, CostType> {

public:
  GridMap(int w, int h, const vector<CostType> &matrix)
      : vertex_matrix_(w, h, matrix),
        node_array_(w, h) {};
  virtual ~GridMap() {};

public:
  static constexpr CostType kDiagonalEdgeWeight = static_cast<CostType>(1.4143);
  static constexpr CostType kStraightEdgeWeight = static_cast<CostType>(1.0);

public:
  static CostType manhattan_distance(const NodeType &n0, const NodeType &n1) {
    return static_cast<CostType>(
        abs(x(n1) - x(n0)) + abs(y(n1) - y(n0)));
  }

  static CostType diagonal_distance(const NodeType &n0, const NodeType &n1) {
    int dx = abs(x(n1) - x(n0));
    int dy = abs(y(n1) - y(n0));
    int dmin = min(dx, dy);
    int dmax = max(dx, dy);
    return static_cast<CostType>(dmin * kDiagonalEdgeWeight
                                 + (dmax - dmin) * kStraightEdgeWeight);
  }

  static CostType duclidean_distance(const NodeType &n0, const NodeType &n1) {
    int dx = abs(x(n1) - x(n0));
    int dy = abs(y(n1) - y(n0));
    return static_cast<CostType>(sqrt(dx*dx + dy*dy));
  }

public:
  void initialize(const NodeType &start, const NodeType &goal) override {
  }

  CostType current_cost(const NodeType &n) const override {
    return node(n)->g_;
  }

  const vector<Edge<NodeType, CostType>> edges(const NodeType &n) override {
    vector <Edge<NodeType, CostType>> es;

    const vector<Coord> &&coords = coord_neighbors(n);

    for (auto c : coords) {
      if (vertex_matrix_.is_passable(c)) {
        es.push_back(Edge<NodeType, CostType>(n, c, edge_cost(n, c)));
      }
    }

    return es;
  }

  bool nodes_equal(const NodeType &n0, const NodeType &n1) const override {
    return n0 == n1;
  }

  bool opon_node_available() override {
    return !open_list_.is_empty();
  }

  bool node_unexplored(const NodeType &n) const override {
    return node(n)->state_ == NodeState::unexplored;
  }

  bool node_open(const NodeType &n) const override {
    return node(n)->state_ == NodeState::open;
  }

  void open_node(NodeType &n, CostType g, CostType h,
                 const NodeType &p) override {
    node(n)->parent_ = node(p);
    node(n)->g_ = g;
    node(n)->f_ = g + h;
    open_list_.insert(node(n));
    node(n)->state_ = NodeState::open;
    stats_.nodes_opened++;
    DEBUG("node inserted: %s", node(n)->to_string().c_str());
  }

  void reopen_node(NodeType &n, CostType g, CostType h,
                   const NodeType &p) override {
    node(n)->parent_ = node(p);
    node(n)->g_ = g;
    node(n)->f_ = g + h;
    open_list_.insert(node(n));
    node(n)->state_ = NodeState::open;
    stats_.nodes_reopened++;
    DEBUG("node reopened: %s", node(n)->to_string().c_str());
  }

  NodeType close_front_open_node() override {
    GridNode<CostType> *gn = open_list_.remove_front();
    gn->state_ = NodeState::closed;
    stats_.nodes_closed++;
    DEBUG("front node removed: %s", gn->to_string().c_str());
    return gn->c_;
  }

  void increase_node_priority(NodeType &n, CostType g, CostType h,
                              const NodeType &p) override {
    node(n)->parent_ = node(p);
    node(n)->g_ = g;
    open_list_.increase_priority(node(n), g + h);
    stats_.nodes_priority_increased++;
    DEBUG("node priority increased: %s", node(n)->to_string().c_str());
  }

  vector<NodeType> get_path(const NodeType &n) override {
    vector<NodeType> path;
    NodeType p = n;
    while (node(p)->parent_->c_ != p){
      path.push_back(p);
      node(p)->state_ = NodeState::result;
      p = node(p)->parent_->c_;
    }
    node(n)->state_ = NodeState::goal;
    node(p)->state_ = NodeState::start;
    return path;
  }

public:
  GridNode<CostType>* node(const NodeType &n) const {
    return node_array_.node(n);
  }

  const string to_string() const {
    static char syms[] = {
        ' ', 'o', '-', '@','S','G'
    };

    ostringstream ss;
    ss << stats_.to_string() + "\n";

    for (int i=0; i < node_array_.h_; i++) {
      for (int j=0; j < node_array_.w_; j++) {
        char c = ' ';
        int v = static_cast<int>(vertex_matrix_.weight(Coord(j,i)));
        if (v < 0)
          c = 'x';
        else {
          c = syms[node_array_.node(Coord(j, i))->state_];
        }
        ss << c;
      }
      ss << '\n';
    }
    return ss.str();
  }

public:
  const VertexMatrix<CostType> vertex_matrix_;
  SearchStats stats_;

protected:
  GridNodeArray<CostType> node_array_;
  HotQueue<GridNode<CostType>*, CostType, GridNode<CostType>,
      BinaryHeap<GridNode<CostType>*, CostType, GridNode<CostType>>> open_list_;

protected:
  static constexpr int x(const NodeType &n) {
    return n.first;
  }
  static constexpr int y(const NodeType &n) {
    return n.second;
  }

protected:
  static const vector<Coord> coord_neighbors(const Coord c) {
    static constexpr int x_offsets_[9] {
        -1, 0, 1,
        -1,    1,
        -1, 0, 1,
    };

    static constexpr int y_offsets_[9] {
        -1,-1,-1,
         0,    0,
         1, 1, 1,
    };

    vector<Coord> cs;
    for (int i=0; i < 8; i++) {
      cs.push_back(Coord(c.first + x_offsets_[i], c.second + y_offsets_[i]));
    }
    return cs;
  }

  static const CostType edge_weight(Coord c0, Coord c1) {
    int d = abs(y(c1) - y(c0)) + abs(x(c1) - x(c0));

    if (d == 2)
      return static_cast<CostType>(kDiagonalEdgeWeight);
    else if (d == 1)
      return static_cast<CostType>(kStraightEdgeWeight);;

    assert(0);
    return 0;
  }

protected:
  CostType edge_cost(const NodeType &n0, const NodeType &n1) const {
    return vertex_matrix_.weight(n1) * edge_weight(n0, n1);
  }

};


#endif /* GRID_MAP_H_ */
