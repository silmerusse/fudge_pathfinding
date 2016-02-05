#ifndef FUDGE_PATHFINDING_GUI_OBJECT_TILE_H_
#define FUDGE_PATHFINDING_GUI_OBJECT_TILE_H_

#include "fruitcandy/core/object.h"

class Realm;

class Tile: public Object {
public:
  Tile() {
    transform_.reset(new Transform());
  }
  virtual ~Tile() = default;

public:
  static std::unique_ptr<Tile> create() {
    return std::move(std::unique_ptr<Tile>(new Tile()));
  }

public:
  std::pair<int, int> pos_;
};

#endif /* FUDGE_PATHFINDING_GUI_OBJECT_TILE_H_ */
