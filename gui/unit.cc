#include "unit.h"
#include "astar_search.h"
#include "grid_map.h"
#include "game.h"

void Unit::tick(Realm *realm) {
  handle_events(realm);

  Game *g = static_cast<Game*>(realm);

  // Pick the next grid position to move into, if available.
  if(UnitState::stop == state_ &&
      get_grid_pos(g) != target_ && !path_.empty()) {
    next_ = path_.back();
    path_.pop_back();
    state_ = moving;

    rotate_to((next_ - get_grid_pos(g)).angle({1, 0}));
  }

  // Move to the next grid position and stop.
  if (UnitState::moving == state_) {
    move_to(g, next_);
    state_ = stop;
  }
}

void Unit::on_mouse_button_down(Realm *realm, unsigned int button,
                                int x, int y) {
  Game *g = static_cast<Game*>(realm);

  if (button == SDL_BUTTON_LEFT) {
    // Set selection status.
    auto position = get_absolute_position();
    if (abs(static_cast<int>(x - position.x)) <= g->grid_width_ / 2 &&
        abs(static_cast<int>(y - position.y)) <= g->grid_height_ / 2) {
      realm->select(this);
      INFO("Unit selected: %s", to_string().c_str());
    }
  } else if (button == SDL_BUTTON_RIGHT) {
    // Perform path searching when this is selected and a new grid position is
    // specified.
    Pos pos (x / g->grid_width_, y / g->grid_height_);
    if (selected_ && get_grid_pos(g) != pos) {
      GridMap<double> map(g->cols_, g->rows_, g->matrix_);
      Pos start = get_grid_pos(g);
      target_ = pos;
      path_ = AStarSearch::search(map, start.to_pair(), target_.to_pair(),
                                  GridMap<double>::diagonal_distance);
      INFO("Path re-calculated.");
    }
  }
}

Pos Unit::get_grid_pos(Game *g) const {
  auto pos = get_absolute_position();
  return Pos(pos.x / g->grid_width_, pos.y / g->grid_height_);
}

void Unit::move_to(Game *g, const Pos &pos) {
  set_position({pos.x * g->grid_width_ + g->grid_width_ / 2,
                pos.y * g->grid_height_ + g->grid_height_ / 2});
}
