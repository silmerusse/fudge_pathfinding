#ifndef WATER_JUG_MAP_H_
#define WATER_JUG_MAP_H_

#include <map>
#include <string>
#include <utility>
#include <sstream>

#include "log.h"

#include "search_stats.h"
#include "node_state.h"
#include "map.h"
#include "hot_queue.h"
#include "priority_queue_stl.h"

#include "edge.h"

class WaterJugNode {
public:
  WaterJugNode(const std::vector<int>& state) : state_(state) {};
  WaterJugNode() {};

public:
  bool operator == (const WaterJugNode &n) const {
    return state_ == n.state_;
  }

public:
  std::vector<int> state_;
  int cost_ = 0;

public:
  const std::string to_string() const {
    std::ostringstream ss;
    for (auto e : state_) {
      ss << e << ',';
    }
    return ss.str();
  }
};

using NodeType = WaterJugNode;

class WaterJugMap : public Map<NodeType, int> {
public:
  WaterJugMap(const std::vector<int>& jugs) : jugs_(jugs) {};
  virtual ~WaterJugMap() {};

public:
  static bool less_priority(const NodeType &a, const NodeType &b) {
    return a.cost_ - b.cost_ > 0;
  }

  static double get_priority(const NodeType &a) {
    return a.cost_;
  }

  static void set_priority(NodeType &a, int v) {
    a.cost_ = v;
  }

public:
  int heuristic(const NodeType n0, const NodeType n1) {
    return 0;
  }

public:
  const std::vector<Edge<NodeType, int>> edges (NodeType &n) override {
    std::vector<Edge<NodeType, int>> edges;

    for (auto i = n.state_.begin(); i != n.state_.end(); ++i) {
      if (*i > 0) {
        int pos = i - n.state_.begin();

        for (auto j = n.state_.begin(); j != n.state_.end(); ++j) {
          int pos1 = j - n.state_.begin();
          if (pos1 != pos) {
            NodeType n1 = n;

            int d = jugs_[pos1] - *j;
            if (d > 0) { // not completely filled
              int dd = std::min(d, *i);
              n1.state_[pos1] += dd;
              n1.state_[pos] -= dd;
              Edge<NodeType, int> edge (n, n1, 1);
              edges.push_back(edge);
            }
          }

        }
      }
    }


    return edges;
  }

  int current_cost(const NodeType &k) const override {
    return k.cost_;
  }

  bool nodes_equal(const NodeType &n0, const NodeType &n1) const override {
    return n0.state_ == n1.state_;
  }

  bool node_unexplored(const NodeType &n) const override {
    return (state_map_.find(n.state_) == state_map_.end());
  }

  bool node_open(const NodeType &n) const override {
    return state_map_.at(n.state_) == NodeState::open;
  }

  bool open_node_available() override {
    return !(open_list_.is_empty());
  }

  // Operations
  void open_node(NodeType &n, int g, int h, const NodeType &p) override {
    parent_map_[n.state_] = p;
    n.cost_ = g + h;
    open_list_.insert(n);
    state_map_[n.state_] = NodeState::open;
    stats_.nodes_opened++;
    DEBUG("node opened: %s, %d", n.to_string().c_str(), n.cost_);
  }

  void reopen_node(NodeType &n, int g, int h, const NodeType &p) override {
    parent_map_[n.state_] = p;
    n.cost_ = g + h;
    open_list_.insert(n);
    state_map_[n.state_] = NodeState::open;
    stats_.nodes_reopened++;
    DEBUG("node reopened: %s, %d", n.to_string().c_str(), n.cost_);
  }

  NodeType close_front_open_node() override {
    NodeType n = open_list_.remove_front();
    state_map_[n.state_] = NodeState::closed;
    stats_.nodes_closed++;
    DEBUG("front node removed: %s, %d", n.to_string().c_str(), n.cost_);
    return n;
  }

  void increase_node_priority(NodeType &n, int g, int h,
                              const NodeType &p) override {
    parent_map_[n.state_] = p;
    n.cost_ = g + h;
    open_list_.increase_priority(n, g + h);
    stats_.nodes_priority_increased++;
    DEBUG("node priority increased: %s, %d", n.to_string().c_str(), n.cost_);
  }

  std::vector<NodeType> get_path(const NodeType &n) override {
    std::vector<NodeType> path;
    NodeType p = n;
    while (parent_map_[p.state_].state_ != p.state_){
      path.push_back(p);
      p = parent_map_[p.state_];
    }
    path.push_back(p);
    return path;
  }

public:
  const std::string to_string() const {
    return stats_.to_string() + "\n";
  }

public:
  SearchStats stats_;

private:
  HotQueue<NodeType, int, WaterJugMap,
      PriorityQueueSTL<NodeType, int, WaterJugMap>> open_list_;
  std::vector<int> jugs_;
  map<std::vector<int>, NodeState> state_map_;
  std::map<std::vector<int>, NodeType> parent_map_;
};

#endif /* WATER_JUG_MAP_H_ */
