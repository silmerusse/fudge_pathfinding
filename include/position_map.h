#ifndef POSITION_MAP_H_
#define POSITION_MAP_H_

#include <vector>
#include <map>
#include <utility>
#include <sstream>
#include "util/log.h"
#include "map.h"
#include "node_state.h"
#include "search_stats.h"
#include "hot_queue.h"
#include "binary_heap.h"

namespace fudge {

template<typename T, typename CostType, typename HashType>
class Position {
public:
  Position(const std::vector<T> &pos): pos_(pos) {};
  Position() {};
  virtual ~Position() = default;

public:
  virtual HashType hash() const = 0;

  virtual const std::string to_string() const = 0;

  virtual bool operator == (const Position<T, CostType, HashType> &pos) const {
  	return hash() == pos.hash();
  }

public:
  std::vector<T> pos_;
  int cost_ = 0;
  int g_ = 0;
  NodeState state_ = NodeState::unexplored;
  HashType parent_;
};

// This implements a position map for solving position based puzzles, in which
// each position of the puzzle could be regarded as a searching node.
template<typename T, typename NodeType, typename CostType, typename HashType>
class PositionMap : public Map<NodeType, int> {
public:
  PositionMap() : open_list_(1) {};
  virtual ~PositionMap() = default;

public:
  static bool less_priority(const NodeType &a, const NodeType &b) {
    return a.cost_ - b.cost_ > 0;
  }

  static double get_priority(const NodeType &a) {
    return a.cost_;
  }

  static void set_priority(NodeType &a, int cost) {
    a.cost_ = cost;
  }

public:
  // Override to return edges with destination nodes according to four moves.
  virtual CostType current_cost(const NodeType &n) const override {
    return map_.at(n.hash()).g_;
  }

  virtual bool nodes_equal(const NodeType &n0,
  												 const NodeType &n1) const override {
    return n0.hash() == n1.hash();
  }

  virtual bool is_node_unexplored(const NodeType &n) const override {
    return map_.find(n.hash()) == map_.end();
  }

  virtual bool is_node_open(const NodeType &n) const override {
    return map_.at(n.hash()).state_ == NodeState::open;
  }

  virtual bool open_node_available() const override {
    return !(open_list_.is_empty());
  }

  // Operations
  virtual void open_node(const NodeType &n, CostType g, CostType h, 
                         const NodeType &p) override {
    NodeType nn(n);
    nn.parent_ = p.hash();
    nn.g_ = g;
    nn.cost_ = g + h;
    nn.state_ = NodeState::open;
    open_list_.insert(nn);
    map_[n.hash()] = nn;
    stats_.nodes_opened++;
    DEBUG("Node opened: %s, %d, %d", nn.to_string().c_str(), nn.g_, nn.cost_);
  }

  virtual void reopen_node(const NodeType &n, int g, int h, 
                           const NodeType &p) override {
    NodeType nn(n);
	  nn.parent_ = p.hash();
	  nn.g_ = g;
	  nn.cost_ = g + h;
	  nn.state_ = NodeState::open;
	  open_list_.insert(nn);
	  map_[nn.hash()] = nn;
	  stats_.nodes_opened++;
	  DEBUG("Node reopened: %s, %d, %d", nn.to_string().c_str(), nn.g_, nn.cost_);
  }

  virtual NodeType take_out_top_node() override {
    NodeType n = open_list_.remove_front();
    map_.at(n.hash()).state_ = NodeState::closed;
    stats_.nodes_closed++;
    DEBUG("Node closed: %s, %d, %d", nn.to_string().c_str(), nn.g_, nn.cost_);
    return n;
  }

  virtual void increase_node_priority(const NodeType &n, CostType g, CostType h,
                                      const NodeType &p) override {
    NodeType &nn = map_.at(n.hash());
    open_list_.increase_priority(nn, g + h);
    nn.parent_ = p.hash();
    nn.g_ = g;
    nn.cost_ = g + h;
    stats_.nodes_priority_increased++;
    DEBUG("Node priority increased: %s, %d, %d", n.to_string().c_str(), 
          n.g_, n.cost_);
  }

  virtual std::vector<NodeType> get_path(const NodeType &n) override {
    std::vector<NodeType> path;
    NodeType p = n;
    while (p.parent_ != p.hash()){
      path.push_back(p);
      p = map_.at(p.parent_);
    }
    path.push_back(p);
    return path;
  }

public:
  const std::string to_string() const {
    return stats_.to_string() + "\n";
  }

public:
  std::map<HashType, NodeType> map_;
  SearchStats stats_;

protected:
  HotQueue<NodeType, CostType, PositionMap,
      BinaryHeap<NodeType, CostType, PositionMap>> open_list_;
};

}

#endif /* POSITION_MAP_H_ */
