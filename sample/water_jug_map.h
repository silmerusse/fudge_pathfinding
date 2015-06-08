#ifndef WATER_JUG_MAP_H_
#define WATER_JUG_MAP_H_

#include <sstream>
#include "position_map.h"

class WaterJugPosition : public Position<int, int, std::string> {
public:
  WaterJugPosition(const std::vector<int> &pos) : Position(pos) {};
  WaterJugPosition() {};

public:
  virtual std::string hash() const override {
    return to_string();
  }

  virtual const std::string to_string() const {
    std::ostringstream ss;
    for (auto &e : pos_) {
      ss << e << ',';
    }
    return ss.str();
  }
};

class WaterJugMap : public PositionMap<std::string, WaterJugPosition, 
                                       int, std::string> {
public:
  WaterJugMap(const std::vector<int> &jugs) : jugs_(jugs) {}
  virtual ~WaterJugMap() = default;

public:
  std::vector<int> jugs_; // The capacity of jugs.

public:
  int heuristic(const WaterJugPosition &n0, const WaterJugPosition &n1) {
    return 0; // We don't have a heuristic yet.
  }

public:
  virtual const std::vector<Edge<WaterJugPosition, int>> 
  edges (WaterJugPosition &n) override {
    std::vector<Edge<WaterJugPosition, int>> edges;

    for (auto i = n.pos_.begin(); i != n.pos_.end(); ++i) {
      if (*i > 0) {
        int pos = i - n.pos_.begin();

        for (auto j = n.pos_.begin(); j != n.pos_.end(); ++j) {
          int pos1 = j - n.pos_.begin();
          if (pos1 != pos) {
            WaterJugPosition n1 = n;

            int d = jugs_[pos1] - *j;
            if (d > 0) { // Not completely filled.
              int dd = std::min(d, *i);
              n1.pos_[pos1] += dd;
              n1.pos_[pos] -= dd;
              Edge<WaterJugPosition, int> edge (n, n1, 1);
              edges.push_back(edge);
            }
          } 
        }
      }
    }
    return edges;
  }

};

#endif /* WATER_JUG_MAP_H_ */
