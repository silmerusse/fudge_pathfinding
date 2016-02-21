#include "airborne.h"
#include "game.h"

Airborne::Airborne() {
  event_target_->register_handler("airborne_move_to",
      std::bind(&Airborne::on_move_to,
          this, std::placeholders::_1));

  event_target_->register_handler("airborne_drop_to",
      std::bind(&Airborne::on_drop_to,
          this, std::placeholders::_1));
}

void Airborne::on_move_to(const std::shared_ptr<Event> &e) {
  int x = e->params_.get_int(0);
  int y = e->params_.get_int(1);
  transform_->set_position({x, y});
}

void Airborne::on_drop_to(const std::shared_ptr<Event> &e) {
  int x = e->params_.get_int(0);
  int y = e->params_.get_int(1);

  Unit *unit =game_->repo_->create_item<Unit>("units");
  unit->game_ = game_;
  Pos pos(x / game_->grid_width_, y / game_->grid_height_);
  unit->move_to(pos);

  auto sprite0 = static_cast<Sprite*>(this->components_[0].get());
  auto sprite1 = std::unique_ptr<Sprite>(new Sprite(*sprite0));

  unit->add_child(std::move(sprite1));

  INFO("Airborne drops to: (%d, %d)", pos.x, pos.y);
  sprite_visible(false);
}
