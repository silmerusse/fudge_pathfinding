#ifndef SLIDING_PUZZLE_MAP_H_
#define SLIDING_PUZZLE_MAP_H_

#include "position_map.h"

class SlidingPosition : public fudge::Position<char, int, std::string> {
public:
  SlidingPosition(const std::string &pos) {
    std::copy(pos.begin(), pos.end(), std::back_inserter(pos_));
  } 
  SlidingPosition() {};
  virtual ~SlidingPosition() = default;

public:
  virtual std::string hash() const override {
    return std::string(pos_.begin(), pos_.end());
  }

  virtual const std::string to_string() const override {
    return hash();
  }
};

class SlidingPuzzleMap : 
    public fudge::PositionMap<char, SlidingPosition, int, std::string> {
public:
  SlidingPuzzleMap(int w, int h) : PositionMap(), w_(w), h_(h) {
    open_list_.kc_ = 2;
  }
  explicit SlidingPuzzleMap(int w) : SlidingPuzzleMap(w, w) {};
  virtual ~SlidingPuzzleMap() = default;

public:
  static constexpr char kHole = '0';

public:
  int manhattan_distance(const SlidingPosition &n0, const SlidingPosition &n1) {
    int distance = 0;
    for (auto y = 0; y < h_; y++) {
      for (auto x = 0; x < w_; x++) {
        char c = n0.pos_[y * w_ + x];
        if (c != kHole) {
          int i = std::find(n1.pos_.begin(), n1.pos_.end(), c) 
              - n1.pos_.begin();
          distance += abs(x - i % w_) + abs(y - i / w_);
        }
      }
    }
    return distance;
  }

public:
  // Override to return edges with destination nodes according to four moves.
  virtual const std::vector<fudge::Edge<SlidingPosition, int>> 
  edges (const SlidingPosition &n) override {
    std::vector<fudge::Edge<SlidingPosition, int>> es;
    int i = std::find(n.pos_.begin(), n.pos_.end(), kHole) - n.pos_.begin();
    int x = i % w_;
    int y = i / w_;

    if (x < w_ - 1)
      push_edge(es, n, i, y * w_ + x + 1); // Move east

    if (x > 0)
      push_edge(es, n, i, y * w_ + x - 1); // Move west

    if (y < h_ - 1)
      push_edge(es, n, i, (y + 1) * w_ + x); // Move south

    if (y > 0)
      push_edge(es, n, i, (y - 1) * w_ + x); // Move north

    return es;
  }

private:
  int w_ = 0;
  int h_ = 0;

private:
  // Helper to create a position node and an edge according to the move.
  // Arguments @i and @j are the index of characters to be swapped.
  // Push the edge to the container.
  void push_edge(std::vector<fudge::Edge<SlidingPosition, int>> &es,
		  	  	 const SlidingPosition &source, int i, int j) {
    SlidingPosition position = source;
    position.pos_[i] = position.pos_[j];
    position.pos_[j] = kHole;
    fudge::Edge<SlidingPosition, int> e(source, position, 1);
    es.push_back(e);
  }
};

constexpr char SlidingPuzzleMap::kHole;

#endif /* SLIDING_PUZZLE_MAP_H_ */
