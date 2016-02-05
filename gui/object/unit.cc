#include "unit.h"
#include "astar_search.h"
#include "grid_map.h"
#include "../game.h"

void Unit::tick(unsigned long ticks) {
  Object::tick(ticks);

  // Pick the next grid position to move into, if available.
  if (UnitState::stop == state_ &&
      get_grid_pos() != target_ && !path_.empty()) {
    next_ = path_.back();
    path_.pop_back();
    state_ = moving;
    transform_->rotate_to((next_ - get_grid_pos()).angle({1, 0}));
  }

  // Move to the next grid position and stop.
  if (UnitState::moving == state_) {
    move_to(next_);
    state_ = stop;
  }
}

void Unit::on_move_to(const std::shared_ptr<Event> &e) {
  int x = e->params_.get_int(0);
  int y = e->params_.get_int(1);
  // Perform path searching when this is selected and a new grid position is
  // specified.
  Pos pos (x / game_->grid_width_, y / game_->grid_height_);
  if (get_grid_pos() != pos) {
    fudge::GridMap<double> map(game_->cols_, game_->rows_, game_->matrix_);
    Pos start = get_grid_pos();
    target_ = pos;
    path_ = fudge::astar_search(map, start.to_pair(), target_.to_pair(),
                                fudge::GridMap<double>::diagonal_distance);
    INFO("Path re-calculated.");
  }
}

Pos Unit::get_grid_pos() const {
  auto pos = get_absolute_position();
  return Pos(pos.x / game_->grid_width_, pos.y / game_->grid_height_);
}

void Unit::move_to(const Pos &pos) {
  transform_->set_position({
    pos.x * game_->grid_width_ + game_->grid_width_ / 2,
    pos.y * game_->grid_height_ + game_->grid_height_ / 2});
}
