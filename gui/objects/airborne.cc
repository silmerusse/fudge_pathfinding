#include "airborne.h"
#include "game.h"

Airborne::Airborne() {
  register_handler("airborne_move_to",
      std::bind(&Airborne::on_move_to,
          this, std::placeholders::_1, std::placeholders::_2));

  register_handler("airborne_drop_to",
      std::bind(&Airborne::on_drop_to,
          this, std::placeholders::_1, std::placeholders::_2));
}

void Airborne::on_move_to(Realm *realm, const std::shared_ptr<Event> &e) {
  int x = e->params_[0]->as_int();
  int y = e->params_[1]->as_int();
  set_position({x, y});
}

void Airborne::on_drop_to(Realm *realm, const std::shared_ptr<Event> &e) {
  int x = e->params_[0]->as_int();
  int y = e->params_[1]->as_int();
  Game *g = static_cast<Game*>(realm);

  Unit *unit =g->object_manager_->create_object<Unit>("units");
  Pos pos (x / g->grid_width_, y / g->grid_height_);
  unit->move_to(g, pos);

  auto sprite0 = static_cast<Sprite*>(this->components_[0].get());
  auto sprite1 = sprite0->clone();
  unit->add_component(std::move(sprite1));

  INFO("Airborne drops to: (%d, %d)", pos.x, pos.y);
  INFO("%s", g->object_manager_->get_store("units")
      ->objects_[1]->to_string().c_str());
  sprite_visible(false);
}
