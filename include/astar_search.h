#ifndef ASTAR_SEARCH_H_
#define ASTAR_SEARCH_H_

#include <vector>
#include "map.h"
#include "edge.h"

class AStarSearch {
public:
  template <typename NodeType, typename CostType, typename Heuristic>
  static std::vector<NodeType> search(Map<NodeType, CostType> &map,
                                      NodeType start, NodeType goal,
                                      Heuristic heuristic) {
    map.open_node(start, 0, heuristic(start, goal), start); // Get started.

    while (map.open_node_available()) {
      NodeType top_node = map.close_front_open_node();

      if (map.nodes_equal(top_node, goal))
        return map.get_path(top_node); // Stop and return the path found.

      const std::vector<Edge<NodeType, CostType>> &&edges = map.edges(top_node);
      for (auto edge : edges) { // For each qualified edge evaluate target node.
        NodeType node_to_evaluate = edge.to_;
        CostType g = map.current_cost(top_node) + edge.cost_;
        CostType h = heuristic(node_to_evaluate, goal);

        if (map.node_unexplored(node_to_evaluate)) {
          map.open_node(node_to_evaluate, g, h, top_node);
        } else if (map.cost_greater(map.current_cost(node_to_evaluate), g)) {
          if (map.node_open(node_to_evaluate)) {
            map.increase_node_priority(node_to_evaluate, g, h, top_node);
          } else { // Won't reach here if heuristic is consistent(monotone).
            map.reopen_node(node_to_evaluate, g, h, top_node);
          }
        }
      }
    }

    return std::vector<NodeType>(); // No path found. Return an empty path.
  }
};


#endif /* ASTAR_SEARCH_H_ */

