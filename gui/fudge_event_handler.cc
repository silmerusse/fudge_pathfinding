#include "fruitcandy/engine/realm.h"
#include "fruitcandy/math/point_in_rect.h"

#include "object/unit.h"
#include "game.h"
#include "fudge_event_handler.h"

void FudgeEventHandler::handle_mouse_button_down(unsigned int button, 
                                                    int x, int y) {
  x_ = x;
  y_ = y;
  Game *game = static_cast<Game*>(realm_);

  DEBUG("InteractionManger::on_mouse_button_down, %d, %d, %d", button, x, y);
  if (button == SDL_BUTTON_LEFT) {
    unselect();
    realm_->repo_->traverse<Unit>("units", std::bind([&](Unit *s) {
          if (s->selectable_) {
            auto rect = s->get_bounding_rect();
            DEBUG("%d, %d, %d, %d", rect.x, rect.y, rect.w, rect.h);
            if (point_in_rect({x, y}, rect)) {
              select(s);
              DEBUG("Unit selected: %s", s->to_string().c_str());;
            }
          }
        }, std::placeholders::_1));
  } else if (button == SDL_BUTTON_RIGHT) {
    // Airborne takes the higher priority.
    if (game->airborne_ && game->airborne_->sprite_visible()) {
      game->airborne_->event_target_->post_event(
          std::make_shared<Event>("airborne_drop_to", x, y));
    } else {
      if (selectee_) {
        Unit *u = static_cast<Unit*>(selectee_);
        std::shared_ptr<Event> e =
            std::make_shared<Event>("unit_move_to", x, y);
        u->event_target_->post_event(e);
        DEBUG("Interaction manager sent out unit_move_to event.");
      }
    }
  }
}

void FudgeEventHandler::handle_mouse_motion(int x, int y, 
                                               int xrel, int yrel) {
  x_ = x;
  y_ = y;
  Game *game = static_cast<Game*>(realm_);

  if (game->airborne_ && game->airborne_->sprite_visible()) {
    game->airborne_->event_target_->post_event(
        std::make_shared<Event>("airborne_move_to", x, y));
  }
}

void FudgeEventHandler::handle_key_down(int keycode, bool repeat) {

  Game *game = static_cast<Game*>(realm_);

  if (keycode == SDLK_b) {
    game->airborne_->sprite_visible(true);
    game->airborne_->event_target_->post_event(
        std::make_shared<Event>("airborne_move_to", x_, y_));
  }
}
