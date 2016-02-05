#ifndef FUDGE_EDGE_H_
#define FUDGE_EDGE_H_

namespace fudge {

template <typename NodeType, typename CostType>
class Edge {
public:
  Edge(NodeType from, NodeType to, CostType cost)
    :from_(from), to_(to), cost_(cost) {};
  virtual ~Edge() {};

public:
  bool operator ==(const Edge &e) const {
    return e.from_ == from_ && e.to_ == to_ && e.cost_ == cost_;
  }


public:
  NodeType from_;
  NodeType to_;
  CostType cost_;
};

}

#endif /* EDGE_H_ */
