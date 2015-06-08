#ifndef TORCHS_PUZZLE_H_
#define TORCHS_PUZZLE_H_

#include <bitset>
#include "position_map.h"

class Torch {
public:
  Torch(const std::vector<int> &controlled): 
      controlled_(controlled) {}
  Torch(const std::initializer_list<int> &l): 
      controlled_(l) {}
public:
  std::vector<int> controlled_; // Indices of other torches being controlled.
  bool on_ = false;
public:
  void swap() {
    on_ = !on_;
  }
};

class TorchesPosition : public Position<Torch, int, char> {
public:
  TorchesPosition(const std::vector<Torch> &torches): Position(torches) {}
  TorchesPosition() {}
  virtual ~TorchesPosition() = default;

public:
  virtual char hash() const override {
    char code = 0;
    for (auto &torch : pos_) {
      (code <<= 1) |= torch.on_ ? 1 : 0;
    }
    return code;
  }

  virtual const std::string to_string() const override {
    std::ostringstream ss;
    ss << std::bitset<7>(hash()) << '-' << static_cast<int>(hash());
    return ss.str();
  }
};

class TorchesPuzzle : public PositionMap<Torch, TorchesPosition, int, char> {
public:
  TorchesPuzzle() = default;
  virtual ~TorchesPuzzle() = default;

public:
  int heuristic(const TorchesPosition &n0, 
                const TorchesPosition &n1) const {
    int d = 0;
    for (auto i = 0; i < n0.pos_.size(); ++i) {
      if (n0.pos_[i].on_ != n1.pos_[i].on_)
        ++d;
    }
    return d;
  }

public:
  // Override to return edges with destination nodes according to four moves.
  virtual const std::vector<Edge<TorchesPosition, int>> 
  edges (TorchesPosition &n) override {
    std::vector<Edge<TorchesPosition, int>> es;
    for (auto i = 0; i < n.pos_.size(); ++i) {
      TorchesPosition pos = n;
      for (auto index : pos.pos_[i].controlled_) {
    	 pos.pos_[index].swap();
      }
      pos.pos_[i].swap();
      es.push_back(Edge<TorchesPosition, int>(n, pos, 1));
    }
    return es;
  }
};

#endif /* TORCHS_PUZZLE_H_ */
