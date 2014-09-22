#ifndef SLIDING_PUZZLE_MAP_H_
#define SLIDING_PUZZLE_MAP_H_

#include <map>
#include <string>
#include <utility>

#define LOG_LEVEL 2
#include "log.h"

#include "search_stats.h"
#include "node_state.h"
#include "map.h"
#include "hot_queue.h"
#include "priority_queue_stl.h"

using namespace std;
using NodeType = pair<string, int>;

// This implements a map for resolving sliding puzzles.
// We use a string(vector of chars) to represent positions.
class SlidingPuzzleMap : public Map<NodeType, int>{
public:
  SlidingPuzzleMap(int w, int h)
      : w_(w), h_(h), open_list_(2) {};
  SlidingPuzzleMap(int w)
      : SlidingPuzzleMap(w, w) {};
  virtual ~SlidingPuzzleMap() {};

public:
  static constexpr char kHole = '0';

public:
  static bool less_priority(const NodeType &a, const NodeType &b) {
    return a.second - b.second > 0;
  }

  static double get_priority(const NodeType &a) {
    return a.second;
  }

  static void set_priority(NodeType &a, int v) {
    a.second = v;
  }

public:
  int manhattan_distance(const NodeType n0, const NodeType n1) {
    int distance = 0;
    for (auto y = 0; y < h_; y++) {
      for (auto x = 0; x < w_; x++) {
        char c = n0.first[y*w_ + x];
        if (c != kHole) {
          int i = n1.first.find(c);
          distance +=  abs(x - i%w_) + abs(y - i/w_);
        }
      }
    }
    return distance;
  }

public:
  // Override to return edges with destination nodes according to four moves.
  const vector<Edge<NodeType, int>> edges (
                                        const pair<string, int> &n) override {
    vector<Edge<NodeType, int>> es;
    int i = n.first.find(kHole);
    int x = i%w_;
    int y = i/w_;

    if (x < w_ - 1)
      push_edge(es, n.first, i, y*w_ + x + 1); // Move east

    if (x > 0)
      push_edge(es, n.first, i, y*w_ + x - 1); // Move west

    if (y < h_ - 1)
      push_edge(es, n.first, i, (y + 1)*w_ + x); // Move south

    if (y > 0)
      push_edge(es, n.first, i, (y - 1)*w_ + x); // Move north

    return es;
  }

  // Not used.
  void initialize (const NodeType &start, const NodeType &end) override {
  }

  int current_cost(const NodeType &k) const override {
    return distance_map_.at(k.first);
  }

  bool nodes_equal(const NodeType &n0, const NodeType &n1) const override {
    return n0.first == n1.first;
  }

  bool node_unexplored(const NodeType &n) const override {
    auto i = state_map_.find(n.first);
    if (i == state_map_.end() || (*i).second == NodeState::unexplored)
      return true;
    else
      return false;
  }

  bool node_open(const NodeType &n) const override {
    auto i = state_map_.find(n.first);
    if (i != state_map_.end() && (*i).second == NodeState::open)
      return true;
    else
      return false;
  }

  bool opon_node_available() override {
    return !(open_list_.is_empty());
  }

  // Operations
  void open_node(NodeType &n, int g, int h, const NodeType &p) override {
    parent_map_[n.first] = p.first;
    distance_map_[n.first] = g;
    n.second = cost_map_[n.first] = g + h;
    open_list_.insert(n);
    state_map_[n.first] = NodeState::open;
    stats_.nodes_opened++;
    DEBUG("node inserted: %s, %d", n.first.c_str(), n.second);
  }

  void reopen_node(NodeType &n, int g, int h, const NodeType &p) override {
    parent_map_[n.first] = p.first;
    distance_map_[n.first] = g;
    n.second = cost_map_[n.first] = g + h;
    open_list_.insert(n);
    state_map_[n.first] = NodeState::open;
    stats_.nodes_reopened++;
    DEBUG("node reopened: %s, %d", n.first.c_str(), n.second);
  }

  NodeType close_front_open_node() override {
    NodeType n = open_list_.remove_front();
    state_map_[n.first] = NodeState::closed;
    stats_.nodes_closed++;
    DEBUG("front node removed: %s, %d", n.first.c_str(), n.second);
    return n;
  }

  void increase_node_priority(NodeType &n, int g, int h,
                              const NodeType &p) override {
    parent_map_[n.first] = p.first;
    distance_map_[n.first] = g;
    open_list_.increase_priority(n, g + h);
    n.second =  cost_map_[n.first] = g + h;
    stats_.nodes_priority_increased++;
    DEBUG("node priority increased: %s, %d", n.first.c_str(), n.second);
  }

  vector<NodeType> get_path(const NodeType &n) override {
    vector<NodeType> path;
    string p = n.first;
    while (parent_map_[p] != p){
      path.push_back(std::make_pair(p, cost_map_[p]));
      state_map_[p] = NodeState::result;
      p = parent_map_[p];
    }
    return path;
  }

public:
  const string to_string() const {
    return stats_.to_string() + "\n";
  }

public:
  map<string, int> distance_map_;
  map<string, int> cost_map_;
  map<string, NodeState> state_map_;
  map<string, string> parent_map_;
  SearchStats stats_;

private:
  int w_ = 0;
  int h_ = 0;
  HotQueue<NodeType, int, SlidingPuzzleMap,
      PriorityQueueSTL<NodeType, int, SlidingPuzzleMap>> open_list_;

private:
  // Helper to create a position node and an edge according to the move.
  // Arguments @i and @j are the index of characters to be swapped.
  // Push the edge to the container.
  void push_edge(vector<Edge<NodeType, int>> &es, const string &source,
                 int i, int j) {
    string position = source;
    position[i] = position[j];
    position[j] = kHole;
    Edge<NodeType, int> e(
        make_pair(source, cost_map_[source]),
        make_pair(position, cost_map_[position]),
        1);
    es.push_back(e);
  }
};

#endif /* SLIDING_PUZZLE_MAP_H_ */
