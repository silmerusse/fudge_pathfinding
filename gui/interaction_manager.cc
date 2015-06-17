#include "interaction_manager.h"
#include "realm.h"
#include "unit.h"
#include "point_in_rect.h"
#include "game.h"

void InteractionManager::on_mouse_button_down(Realm *realm,
    const std::shared_ptr<Event> &e) {
  unsigned int button = e->params_[0]->as_uint();
  x_ = e->params_[1]->as_int();
  y_ = e->params_[2]->as_int();

  Game *g = static_cast<Game*>(realm);

  DEBUG("InteractionManger::on_mouse_button_down, %d, %d, %d", button, x_, y_);
  if (button == SDL_BUTTON_LEFT) {
    unselect();
    realm->object_manager_->traverse_store<Unit>("units",
        [&](Unit *s) {
          if (s->selectable_) {
            auto rect = s->get_bounding_rect();
            DEBUG("%d, %d, %d, %d", rect.x, rect.y, rect.w, rect.h);
            if (point_in_rect({x_, y_}, rect)) {
              select(s);
              DEBUG("Unit selected: %s", s->to_string().c_str());;
            }
          }
        }
    );
  } else if (button == SDL_BUTTON_RIGHT) {
    // Airborne takes the higher priority.
    if (g->airborne_ && g->airborne_->sprite_visible()) {
      g->airborne_->post_event(
          std::make_shared<Event>("airborne_drop_to", x_, y_));
    } else {
      if (selectee_) {
        Unit *u = static_cast<Unit*>(selectee_);
        std::shared_ptr<Event> e =
            std::make_shared<Event>("unit_move_to", x_, y_);
        u->post_event(e);
        DEBUG("Interaction manager sent out unit_move_to event.");
      }
    }
  }
}

void InteractionManager::on_mouse_motion(Realm *realm,
    const std::shared_ptr<Event> &e) {
  x_ = e->params_[0]->as_int();
  y_ = e->params_[1]->as_int();

  Game *g = static_cast<Game*>(realm);

  if (g->airborne_ && g->airborne_->sprite_visible()) {
    g->airborne_->post_event(
        std::make_shared<Event>("airborne_move_to", x_, y_));
  }
}

void InteractionManager::on_key_down(Realm *realm,
    const std::shared_ptr<Event> &e) {
  int sym = e->params_[0]->as_int();

  Game *g = static_cast<Game*>(realm);

  if (sym == SDLK_b) {
    g->airborne_->sprite_visible(true);
    g->airborne_->post_event(
        std::make_shared<Event>("airborne_move_to", x_, y_));
  }
}
