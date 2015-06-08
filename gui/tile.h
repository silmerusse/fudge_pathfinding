#ifndef TILE_H_
#define TILE_H_

#include "object.h"

class Realm;

class Tile: public Object {
public:
  Tile() = default;
  virtual ~Tile() = default;

public:
  static std::unique_ptr<Tile> create() {
    return std::move(std::unique_ptr<Tile>(new Tile()));
  }

  virtual void load(const Json::Value& v) override {};

public:
  std::pair<int, int> pos_;
};

#endif /* TILE_H_ */
