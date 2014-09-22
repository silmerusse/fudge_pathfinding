#ifndef EDGE_H_
#define EDGE_H_

template <typename NodeType, typename CostType>
class Edge {
public:
  Edge(NodeType from, NodeType to, CostType cost)
    :from_(from), to_(to), cost_(cost) {};
  virtual ~Edge() {};

public:
  NodeType from_;
  NodeType to_;
  CostType cost_;
};

#endif /* EDGE_H_ */
