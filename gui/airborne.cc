#include "airborne.h"
#include "game.h"

Airborne::Airborne() {}

void Airborne::on_mouse_motion(Realm *realm, int x, int y, int relx, int rely) {
  Game *g = static_cast<Game*>(realm);
  set_position({static_cast<double>(x), static_cast<double>(y)});
}

void Airborne::on_key_down(Realm *realm, int sym) {
  if (sym == SDLK_b) {
    sprite_visible(true);
  }
}

void Airborne::on_mouse_button_down(Realm *realm, unsigned int button,
                                    int x, int y) {
  if (button == SDL_BUTTON_LEFT && sprite_visible()) {
    Game *g = static_cast<Game*>(realm);

    Unit *unit =g->object_manager_->create_object<Unit>("units");
    Pos pos (x / g->grid_width_, y / g->grid_height_);
    unit->move_to(g, pos);

    auto sprite0 = static_cast<Sprite*>(this->components_[0].get());
    auto sprite1 = sprite0->clone();
    unit->add_component(std::move(sprite1));

    // Register listener so that they could know if being selected.
    g->event_queue_.register_listener("mouse_button_down", unit);

    INFO("Airborne drops to: (%d, %d)", pos.x, pos.y);
    INFO("%s", g->object_manager_->get_store("units")->objects_[1]->to_string().c_str());
    sprite_visible(false);
  }
}
