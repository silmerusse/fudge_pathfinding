#ifndef FUDGE_ASTAR_SEARCH_H_
#define FUDGE_ASTAR_SEARCH_H_

#include <vector>
#include "map.h"
#include "edge.h"

namespace fudge {

template <typename NodeType, typename CostType, typename Heuristic>
std::vector<NodeType> astar_search(Map<NodeType, CostType> &map, 
                                   const NodeType &start, const NodeType &goal, 
                                   Heuristic heuristic) {
    // Open the start node to get started.
    map.open_node(start, 0, heuristic(start, goal), start); 

    while (map.open_node_available()) {
      const NodeType top_node = map.take_out_top_node();

      // If the top node is the goal, stop search and return the path.
      if (map.nodes_equal(top_node, goal))
        return map.get_path(top_node); 

      // Evaluate neighbor nodes for each valid edge.
      const std::vector<Edge<NodeType, CostType>> edges = map.edges(top_node);
      for (auto edge : edges) {
        // Calculate cost of the neighor node.
        const NodeType node_to_evaluate = edge.to_;
        CostType g = map.current_cost(top_node) + edge.cost_;
        CostType h = heuristic(node_to_evaluate, goal);

        // If we found an unexplored node, add it to the open list.
        // Otherwise, if the new cost is lower, refresh the cost of the node 
        // as we just found a shorter path.
        if (map.is_node_unexplored(node_to_evaluate)) {
          map.open_node(node_to_evaluate, g, h, top_node);
        } else if (map.cost_less(g, map.current_cost(node_to_evaluate))) {
          if (map.is_node_open(node_to_evaluate)) {
            map.increase_node_priority(node_to_evaluate, g, h, top_node);
          } else { 
            // Won't reach here if the heuristic is consistent(monotone).
            map.reopen_node(node_to_evaluate, g, h, top_node);
          }
        }
      }
    }

    return std::vector<NodeType>(); // No path found. Return an empty path.
}

}

#endif /* FUDGE_ASTAR_SEARCH_H_ */

