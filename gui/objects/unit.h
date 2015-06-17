#ifndef UNIT_H_
#define UNIT_H_

#include <memory>
#include <vector>
#include "object.h"
#include "vector2D.h"
#include <SDL2/SDL.h>

class Game;

using Pos = Vector2D<int>;
enum UnitState {stop, moving};

class Unit: public Object {
public:
  Unit() {
    selectable_ = true;
    register_handler("unit_move_to",
        std::bind(&Unit::on_move_to,
            this, std::placeholders::_1, std::placeholders::_2));
  }
  virtual ~Unit() = default;

public:
  static std::unique_ptr<Unit> create() {
    return std::move(std::unique_ptr<Unit>(new Unit()));
  }

  virtual void tick(Realm *realm) override;

  virtual void on_move_to(Realm *realm, const std::shared_ptr<Event> &e);

  // Get the current grid position.
  Pos get_grid_pos(Game *g) const;

  // Move to the specified grid position.
  void move_to(Game *g, const Pos &pos);

  const SDL_Rect get_bounding_rect() {
    auto pos = get_absolute_position();
    return SDL_Rect{static_cast<int>(pos.x - w_ / 2),
                    static_cast<int>(pos.y - h_ / 2),
                    w_, h_};
  }
public:
  int w_ = 32;
  int h_ = 32;
  UnitState state_ = stop;  // Current state.
  Pos target_;              // Target postion to move into.
  Pos next_;                // Next position along the path to move into.
  std::vector<std::pair<int, int>> path_;   // Planned moving path.
};

#endif /* UNIT_H_ */
