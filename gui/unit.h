#ifndef UNIT_H_
#define UNIT_H_

#include <memory>
#include <vector>
#include "object.h"
#include "vector2D.h"

class Game;

using Pos = Vector2D<int>;
enum UnitState {stop, moving};

class Unit: public Object {
public:
  Unit() = default;
  virtual ~Unit() = default;

public:
  static std::unique_ptr<Unit> create() {
    return std::move(std::unique_ptr<Unit>(new Unit()));
  }

  virtual void load(const Json::Value& v) override {
  }

  virtual void tick(Realm *realm) override;

  virtual void on_mouse_button_down(Realm *realm, unsigned int button,
                                    int x, int y) override;

  // Get the current grid position.
  Pos get_grid_pos(Game *g) const;

  // Move to the specified grid position.
  void move_to(Game *g, const Pos &pos);

public:
  UnitState state_ = stop;  // Current state.
  Pos target_;              // Target postion to move into.
  Pos next_;                // Next position along the path to move into.
  std::vector<std::pair<int, int>> path_;   // Planned moving path.
};

#endif /* UNIT_H_ */
