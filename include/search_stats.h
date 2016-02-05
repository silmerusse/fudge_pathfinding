#ifndef FUDGE_SEARCH_STATS_H_
#define FUDGE_SEARCH_STATS_H_

#include <string>
#include <sstream>

namespace fudge {

class SearchStats {
public:
  int nodes_opened = 0;
  int nodes_closed = 0;
  int nodes_reopened = 0;
  int nodes_priority_increased = 0;

public:
  void reset() {
    nodes_opened = 0;
    nodes_closed = 0;
    nodes_reopened = 0;
    nodes_priority_increased = 0;
  }

  const std::string to_string() const {
    std::ostringstream ss;
    ss << "  nodes opened:" << nodes_opened << '\n'
       << "  nodes closed:" << nodes_closed << '\n'
       << "  nodes priority increased:" << nodes_priority_increased << '\n'
       << "  nodes reopened:" << nodes_reopened << '\n';
    return ss.str();
  }
};

}

#endif /* FUDGE_SEARCH_STATS_H_ */
