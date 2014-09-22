#ifndef SEARCH_STATS_H_
#define SEARCH_STATS_H_

#include <string>
#include <sstream>

using namespace std;

class SearchStats
{
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

  const string to_string() const {
    stringstream ss;
    ss << "  nodes opened:" << nodes_opened << '\n'
       << "  nodes closed:" << nodes_closed << '\n'
       << "  nodes priority increased:" << nodes_priority_increased << '\n'
       << "  nodes reopened:" << nodes_reopened << '\n';
    return ss.str();
  }
};
#endif /* SEARCH_STATS_H_ */
