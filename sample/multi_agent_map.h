#ifndef MULTI_AGENT_MAP_H_
#define MULTI_AGENT_MAP_H_

#include <string>
#include <utility>
#include <sstream>
#include <deque>
#include <queue>
#include <memory>
#include <unordered_map>
#include <functional>

#include "log.h"

#include "search_stats.h"
#include "node_state.h"
#include "grid_map.h"
#include "hot_queue.h"
#include "priority_queue_stl.h"
#include "edge.h"
#include "rra.h"

// Use uint_8 based on the assumption that the maximal map dimension is 256x256.
using Pos = std::pair<uint8_t, uint8_t>;

enum class AgentState {unmoved, moved};

// Represent a moving agent.
class Agent {
public:
  Agent(uint8_t id, Pos start, Pos end, uint8_t speed = 1,
      uint8_t predecessor = 255):
    id_(id), start_(start), pos_(start), end_(end), speed_(speed),
    cd_(speed - 1), predecessor_(predecessor) {};

public:
  uint8_t id_;      // Unique ID ranged in 0~254.
  Pos start_;       // Start position.
  Pos pos_;         // Current position.
  Pos end_;         // End/Target position.
  uint8_t speed_;   // Turns required to move to a nearby grid.
  uint8_t cd_;      // Turns remained to be able to move again.
  uint8_t predecessor_; // Predecessor who has to move first.
  AgentState state_ = AgentState::unmoved;

public:
  // Calculate and save hash code.
  std::size_t hash () const {
    std::size_t code = id_ + 1;
    code = code * 31 + cd_;
    code = code * 31 + pos_.first;
    code = code * 31 + pos_.second;
    return code;
  }

  bool operator ==(const Agent &a) const {
    return id_ == a.id_ && cd_ == a.cd_ && pos_ == a.pos_;
  }

  // Generate a human friendly string for debug purpose.
  const std::string to_string() const {
    std::ostringstream ss;
    ss << static_cast<char>('A' + id_)
       << static_cast<unsigned int>(cd_)  << '|'
       << '(' << static_cast<unsigned int>(pos_.first) << ','
              << static_cast<unsigned int>(pos_.second) << ')';
    return ss.str();
  }
};

// Represent an agent's move.
class Move {
public:
  Move(const Agent &agent, Pos to): agent_(agent), to_(to) {}

public:
  Agent agent_;
  Pos to_;

public:
  // Calculate and save hash code.
  std::size_t hash() const {
    return (agent_.hash() * 31  + to_.first) * 31 + to_.second ;
  }

  bool operator ==(const Move &m) const {
    return agent_ == m.agent_ && to_ == m.to_;
  }

  // Generate a human friendly string for debug purpose.
  const std::string to_string() const {
    std::ostringstream ss;
    ss << static_cast<char>('A' + static_cast<unsigned int>(agent_.id_))
       << static_cast<unsigned int>(agent_.cd_) << '|'
       << '(' << static_cast<unsigned int>(agent_.pos_.first) << ','
              << static_cast<unsigned int>(agent_.pos_.second) << ')'
              << "->"
              << '(' << static_cast<unsigned int>(to_.first)
              << ',' << static_cast<unsigned int>(to_.second) << ')';
    return ss.str();
  }
};


// Represent a position/state of all agents on the map.
class MultiAgentNode {
public:
  MultiAgentNode(const std::deque<Move> &planned = {},
                    const std::deque<Move> &expanded = {},
                    const std::deque<Agent> &unplanned = {}):
      planned_(planned), expansion_list_(expanded), unplanned_(unplanned),
      hash_(hash()) {};
  virtual ~MultiAgentNode() = default;

public:
  std::deque<Move> planned_;        // List of moves to be performed.
  std::deque<Move> expansion_list_; // List of moves expanded.
  std::deque<Agent> unplanned_;     // List of unplanned agents.

  // The actual cost from start to current position.
  int g_ = 0;

  // Current total cost(g + h). This should be the sum of actual + estimated
  // turns of all living agents. When an agent reached its goal, it will not
  // contribute to this anymore.
  int cost_ = 0;

  // Hash code for indexing.
  std::size_t hash_ = 0;

  // Indicate whether the node is an end-of-turn node. If it is, moves in
  // planned list should be calculated and all agents shoudl be moved into
  // unplanned list for next turn.
  bool end_of_turn_ = false;

  // The debug friendly annotation presentation of the node.
  std::string annotation_;

  // Pointer to the parent node. This will be used to trace the result path.
  std::shared_ptr<MultiAgentNode> parent_;

  // State of the node.
  NodeState state_ = NodeState::unexplored;

  // Indicate whether this has been expanded. After it's expanded, the possible
  // moves of the first Agent in the unplanned list should be added to expanded
  // list.
  bool expanded_ = false;


public:
  // Factory method to create a new instance.
  static std::shared_ptr<MultiAgentNode> create() {
    return std::make_shared<MultiAgentNode>();
  }

  // Factory method to create a new instance.
  static std::shared_ptr<MultiAgentNode> create(
      const std::deque<Move> &planned,
      const std::deque<Move> &expanded,
      const std::deque<Agent> &unplanned) {
    return std::make_shared<MultiAgentNode>(planned, expanded, unplanned);
  }

  // Generate a hash code for indexing. The code is not unique. Therefore, it
  // might collide with others. An instance equal check should be performed
  // when the collision happens.
  const std::size_t hash() {
    std::size_t code =0;

    for (const Move &move : planned_) {
      code = (code * 31 ) + move.hash();
    }

    std::deque<Agent> v = unplanned_;
    std::sort(v.begin(), v.end(), [](const Agent &a, const Agent &b) {
      return a.id_ < b.id_;
    });

    for (const Agent &a : v) {
      code = code * 31 +  a.hash();
    }
    hash_ = code;
    return code;
  }

  // Generate a human friendly string for debug purpose.
  const std::string to_string() const {
    std::ostringstream ss;
    ss << '[';
    for (auto m = planned_.begin(); m != planned_.end(); ++m) {
      if (m != planned_.begin())
        ss <<" ";
      ss << m->to_string();
    }
    ss << ']';
    ss << '<';
    for (auto m = expansion_list_.begin(); m != expansion_list_.end(); ++m) {
      if (m != expansion_list_.begin())
        ss <<" ";
      ss << m->to_string();
    }
    ss << '>';
    ss << '{';
    for (auto a = unplanned_.begin(); a != unplanned_.end(); ++a) {
      if (a != unplanned_.begin())
        ss <<" ";
      ss << a->to_string();
    }
    ss << '}';
    return ss.str();
  }

public:
  bool operator == (const MultiAgentNode &n) const {
    return planned_ == n.planned_ && unplanned_ == n.unplanned_;
  }
};

// An instance equal check when collision happens.
struct NodeEqual {
 bool operator()(const std::shared_ptr<MultiAgentNode> &x,
     const std::shared_ptr<MultiAgentNode> &y) const {
   if (x->hash_ == y->hash_ && x.get() != y.get()) {
     DEBUG("Collision: %s --- %s", x->annotation_.c_str(),
                                   y->annotation_.c_str());
   }
   return *(x.get()) == *(y.get());
 };
};

// Use custom hash code.
struct NodeHash {
 std::size_t operator()(const std::shared_ptr<MultiAgentNode> &x) const {
   return x->hash_;
 };
};

class MultiAgentMap : public Map<std::shared_ptr<MultiAgentNode>, int> {

  using NodeType = std::shared_ptr<MultiAgentNode>;

public:
  MultiAgentMap(int w, int h, const std::vector<int> &matrix,
      double weight = 1.0):
    open_list_(), grid_map_(w, h, matrix), rra_(matrix, w, h),
    weight_(weight) {};
  virtual ~MultiAgentMap() = default;

public:
  // This is used to compare node priorities in open list.
  static bool less_priority(const NodeType &a, const NodeType &b) {
    return a->cost_ - b->cost_ > 0;
  }

  static double get_priority(const NodeType &a) {
    return a->cost_;
  }

  static void set_priority(NodeType &a, int v) {
    a->cost_ = v;
  }

public:


  // Use RRA* (Reverse Resumable A*) algorithm to get more accurate heuristic.
  int heuristic_rra(const NodeType n0, const NodeType n1) {
    int sum = 0;
    for (auto i = n0->planned_.begin(); i != n0->planned_.end(); ++i) {
      int d = rra_.search(i->agent_.end_, i->to_,
          GridMap<int>::manhattan_distance);
      sum += d * i->agent_.speed_;
    }
    for (auto i = n0->unplanned_.begin(); i != n0->unplanned_.end(); ++i) {
      Agent a = *i;
      int d = rra_.search(a.end_, a.pos_, GridMap<int>::manhattan_distance);
      sum += d * a.speed_;
    }
    return sum * weight_;
  }

  // The simplest heuristic is the sum of the Manhattan distance of all agents.
  int heuristic_manhattan(const NodeType n0, const NodeType n1) {
    int sum = 0;
    for (auto i = n0->planned_.begin(); i != n0->planned_.end(); ++i) {
      int d = GridMap<int>::manhattan_distance(i->to_, i->agent_.end_);
      sum += d * i->agent_.speed_;
    }
    for (auto i = n0->unplanned_.begin(); i != n0->unplanned_.end(); ++i) {
      Agent a = *i;
      int d = GridMap<int>::manhattan_distance(a.pos_, a.end_);
      sum += d * a.speed_;
    }
    return sum * weight_;
  }

public:
  // Generate all valid edges for the node.
  const std::vector<Edge<NodeType, int>> edges (NodeType &from) override {
    std::vector<Edge<NodeType, int>> result;

    // Expand the parent node if it's not expanded.
    if (!from->expanded_&& !from->unplanned_.empty()) {
        Agent a = from->unplanned_.front();
        from->expansion_list_ = possible_moves(a);
        from->expanded_ = true;
    }

    int count = 2; // x nodes to expand at a time.
    while (count > 0 && !from->expansion_list_.empty()) {
      Move move = from->expansion_list_.front();
      from->expansion_list_.pop_front();

      if (is_legal(move, from)) {
        NodeType to = MultiAgentNode::create();
        // Inherit planned moves and unplanned agents from parent.
        to->planned_ = from->planned_;
        to->planned_.push_back(move);
        to->unplanned_ = from->unplanned_;
        if (!to->unplanned_.empty()) {
          to->unplanned_.pop_front();
        }

        // Calculate all moves and prepare for next turn
        // if this is the end of a turn.
        if (to->unplanned_.empty()) {
          to->end_of_turn_ = true;
          to->annotation_ = to->to_string();

          // Move nodes to unplanned list except those nodes that reach their
          // targets and should be removed.
          while (!to->planned_.empty()) {
            Move &move = to->planned_.front();
            Agent a = move.agent_;
            a.pos_ = move.to_;

            if (a.pos_ != a.start_)
              a.state_ = AgentState::moved;
            if (a.pos_ != a.end_) {
              to->unplanned_.push_back(a);
            }
            to->planned_.pop_front();
          }
        }

        --count;

        // Generate a new edge.
        to->hash();
        result.push_back(Edge<NodeType, int>(from, to, 1));
      }
    }

    // If the parent node still has expanded moves, put it back to open list.
    if (!from->expansion_list_.empty()) {
      int h = from->cost_ - from->g_; // Workaround
      open_node(from, from->g_, h, from->parent_);
    }

    return result;
  }

  // Return the current least cost of the (same) node.
  int current_cost(const NodeType &n) const override {
    // This is called when there's at least one node explored.
    return map_.find(n)->second->g_;
  }

  // This is called when to compare a node to target node.
  bool nodes_equal(const NodeType &n0, const NodeType &n1) const override {
    return *n0 == *n1;
  }

  bool node_unexplored(const NodeType &n) const override {
    if (map_.find(n) != map_.end())
      return false;
    return true;
  }

  bool node_open(const NodeType &n) const override {
    return map_.find(n)->second->state_ == NodeState::open;
  }

  bool open_node_available() const override {
    return !(open_list_.is_empty());
  }

  void open_node(NodeType &n, int g, int h, const NodeType &p) override {
    n->parent_ = p;
    n->cost_ = g + h;
    n->g_ = g;
    open_list_.insert(n);
    n->state_ = NodeState::open;
    ++stats_.nodes_opened;

    // Add this to prevent an expanding node to add multiple times to open list.
    if (node_unexplored(n)) {
      map_.emplace(n, n);
    }

    DEBUG("Node opened: %40s(%d)\t0x%zx \t<- %20s(%d)\t0x%zx",
              n->to_string().c_str(), n->cost_, n->hash_,
              p->to_string().c_str(), p->cost_, p->hash_);

    if (stats_.nodes_opened%50000 == 0) {
      WARN("Total nodes opened: %d; Map contains: %zd; In open list: %zd",
          stats_.nodes_opened, map_.size(), open_list_.size());
    }
  }

  void reopen_node(NodeType &n, int g, int h, const NodeType &p) override {
    n->parent_ = p;
    n->cost_ = g + h;
    n->g_ = g;
    open_list_.insert(n);
    n->state_ = NodeState::open;
    ++stats_.nodes_reopened;

    map_.emplace(n, n);
    DEBUG("Node reopened: %s(%d)\t0x%zx <- %s(%d)\t0x%zx",
        n->to_string().c_str(), n->cost_, n->hash_,
        p->to_string().c_str(), p->cost_, p->hash_);
  }

  NodeType close_front_open_node() override {
    NodeType n = open_list_.front();
    open_list_.remove_front();
    n->state_ = NodeState::closed;
    ++stats_.nodes_closed;
    auto p = n->parent_;

    DEBUG("Node removed: %40s(%d)\t0x%zx \t<- %20s(%d)\t0x%zx",
        n->to_string().c_str(), n->cost_, n->hash_,
        p->to_string().c_str(), p->cost_, p->hash_);
    return n;
  }

  void increase_node_priority(NodeType &n, int g, int h,
                              const NodeType &p) override {
    auto o = map_.find(n)->second;
    o = n;
    o->parent_ = p;
    o->cost_ = g + h;
    o->g_ = g;

    o->state_ = NodeState::open;
    ++stats_.nodes_opened;

    map_.emplace(o, o);
    DEBUG("Node priority increased: %s(%d)", n->to_string().c_str(), n->cost_);
  }

  std::vector<NodeType> get_path(const NodeType &n) override {
    std::vector<NodeType> path;
    auto p = n;
    while (p->parent_ != p){
      path.push_back(p);
      p = p->parent_;
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
  // The performance of hot queue should be better than
  // a patched STL priority queue.
  HotQueue<NodeType, int, MultiAgentMap,
      PriorityQueueSTL<NodeType, int, MultiAgentMap>> open_list_;
  //PriorityQueueSTL<NodeType, int, MovingObjectsMap> open_list_;

  GridMap<int> grid_map_;
  std::unordered_multimap<NodeType, NodeType, NodeHash, NodeEqual> map_;
  RRA<int> rra_;
  double weight_;

private:
  std::deque<Move> possible_moves(const Agent &agent) {
    std::deque<Move> moves;

    int x = agent.pos_.first;
    int y = agent.pos_.second;

    // When an agent's CD reachs zero, it's allowed to move.
    if (agent.cd_ <= 0) {
      for (auto c: {Pos(x + 1, y), Pos(x - 1, y),
                    Pos(x, y + 1), Pos(x, y - 1)}){
        if (grid_map_.vertex_matrix_.is_passable(c)) {
          Agent a = agent;
          a.cd_ = a.speed_ - 1;
          moves.push_back(Move(a, c));
        }
      }
    }

    // It's always OK for an agent to stand still.
    Agent a = agent;
    a.cd_ = a.cd_ <= 0 ? 0 : a.cd_ - 1;
    moves.push_back(Move(a, Pos(x, y)));

    // Sort to expand from the highest possible direction.
    std::sort(moves.begin(), moves.end(), [&](const Move &a, const Move &b) {
      //return GridMap<int>::manhattan_distance(a.to_, a.agent_.end_)
      //    < GridMap<int>::manhattan_distance(b.to_, b.agent_.end_);
      return rra_.search(a.agent_.end_, a.to_, GridMap<int>::manhattan_distance)
          < rra_.search(a.agent_.end_, a.to_, GridMap<int>::manhattan_distance);
    });

    return moves;
  }

  bool is_legal(const Move &move, const NodeType &n) {
    // It's OK to stand still at the start grid even if there's any unmoved
    // node in the same grid, except there's any other node wants to move to it.
    if (move.to_ == move.agent_.start_) { // Additional rule.
      for (auto m: n->planned_) {
        if (m.to_ == move.to_ && m.to_ != m.agent_.start_)
          return false;
      }
      return true;
    }

    // Move face to face is not allowed.
    for (auto m: n->planned_) {
      if (m.to_ == move.to_ ||
           (m.agent_.pos_ == move.to_ && m.to_ == move.agent_.pos_)) {
        return false;
      }

      // If a node's predecessor is not moved, the node not allowed to move.
      if (m.agent_.id_ == move.agent_.predecessor_
          && m.agent_.state_ == AgentState::unmoved)
        return false;
    }

    // Check the agents in unplanned list, too.
    // If a node's predecessor is not moved, the node is not allowed to move.
    for (auto a: n->unplanned_) {
      if (a.id_ == move.agent_.predecessor_ && a.state_ == AgentState::unmoved)
        return false;
    }
    return true;
  }
};

#endif /* MULTI_AGENT_MAP_H_ */
