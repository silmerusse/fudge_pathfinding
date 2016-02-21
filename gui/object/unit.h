#ifndef FUDGE_PATHFINDING_GUI_OBJECT_UNIT_H_
#define FUDGE_PATHFINDING_GUI_OBJECT_UNIT_H_

#include <memory>
#include <vector>
#include <SDL2/SDL.h>

#include "fruitcandy/engine/object.h"
#include "fruitcandy/math/vector2D.h"

class Game;

using Pos = Vector2D<int>;
enum UnitState {stop, moving};

class Unit: public Object {
public:
  Unit() {
    transform_.reset(new Transform());
    event_target_.reset(new EventTarget());
    event_target_->register_handler("unit_move_to",
        std::bind(&Unit::on_move_to, this, std::placeholders::_1));
    selectable_ = true;
  }
  virtual ~Unit() = default;

public:
  static std::unique_ptr<Unit> create() {
    return std::move(std::unique_ptr<Unit>(new Unit()));
  }

  virtual void tick(unsigned long ticks) override;

  virtual void on_move_to(const std::shared_ptr<Event> &e);

  // Get the current grid position.
  Pos get_grid_pos() const;

  // Move to the specified grid position.
  void move_to(const Pos &pos);

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
  Game *game_ = nullptr;
};

#endif /* FUDGE_PATHFINDING_GUI_OBJECT_UNIT_H_ */
