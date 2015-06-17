#include "camera.h"
#include "sprite.h"
#include "renderable.h"
#include "object.h"

Camera::Camera(const SDL_Rect& rect, Realm *realm,
               const std::vector<std::string> &store_names) :
    rect_(rect), realm_(realm), store_names_(store_names) {
}

void Camera::shoot() {
  renderables_.clear();
  for (auto store_name : store_names_) {
    realm_->object_manager_->traverse_store<Object>(
        store_name, [&](Object *object) {
          for (auto &component: object->components_) {
            if (dynamic_cast<Renderable*>(component.get())) {
              Renderable *r = dynamic_cast<Renderable*>(component.get());
              auto rect = r->get_rect();
              if (r->visible_ && SDL_HasIntersection(&rect, &rect_)) {
                renderables_.push_back(r);
              }
            }
          }
    });
  }
}

void Camera::dispatch_event(const SDL_Event &e) {
  switch (e.type) {
  case SDL_KEYDOWN:
    handle_key_down(e.key);
    break;
  case SDL_MOUSEBUTTONDOWN:
    handle_mouse_button_down(e.button);
    break;
  case SDL_MOUSEMOTION:
    handle_mouse_motion(e.motion);
    break;
  default:
    break; // Discard other events.
  }
}

void Camera::handle_key_down(const SDL_KeyboardEvent &e) {
  realm_->event_hub_.push_back(
      std::move(Event::create("key_down", e.keysym.sym)));

  switch (e.keysym.sym) {
  case SDLK_LEFT:
    move_by(-1, 0);
    break;
  case SDLK_RIGHT:
    move_by(1, 0);
    break;
  case SDLK_UP:
    move_by(0, -1);
    break;
  case SDLK_DOWN:
    move_by(0, 1);
    break;
  }
}

void Camera::handle_mouse_button_down(const SDL_MouseButtonEvent &e) {
  realm_->event_hub_.push_back(
      std::move(Event::create("mouse_button_down",
                              e.button,
                              e.x + rect_.x,
                              e.y + rect_.y)));
}

void Camera::handle_mouse_motion (const SDL_MouseMotionEvent &e) {
  realm_->event_hub_.push_back(
      std::move(Event::create("mouse_motion",
                                    e.x + rect_.x,
                                    e.y + rect_.y,
                                    e.xrel, e.yrel)));
}
