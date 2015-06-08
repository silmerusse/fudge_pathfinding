#include <jsoncpp/json/json.h>
#include "log.h"
#include "viewport.h"

Viewport::Viewport(const SDL_Rect& rect, int z_order) :
    Panel(rect, z_order) {
}

void Viewport::observe(std::unique_ptr<Camera> &&camera) {
  camera_ = std::move(camera);
}

void Viewport::render(SDL_Renderer *renderer) const {
  Panel::render(renderer);

  camera_->shoot();

  Vector2D<int> offset{-camera_->rect_.x, -camera_->rect_.y};
  // Draw each sprite visible in the camera.
  for (const auto &renderable : camera_->renderables_)
    renderable->render(renderer, offset);
}

void Viewport::handle_event(const SDL_Event &e) {
  switch (e.type) {
  case SDL_KEYDOWN:
    handle_key_down(e.key);
    break;
  case SDL_MOUSEBUTTONDOWN:
    handle_mouse_button_down(e.button);
    break;
  case SDL_MOUSEMOTION:
    handle_mouse_motion(e.motion);
  }
}

void Viewport::handle_key_down(const SDL_KeyboardEvent &e) {
  camera_->realm_->event_queue_.push_back(
      std::move(Event::create("key_down", e.keysym.sym)));

  switch (e.keysym.sym) {
  case SDLK_LEFT:
    camera_->move_by(-1, 0);
    break;
  case SDLK_RIGHT:
    camera_->move_by(1, 0);
    break;
  case SDLK_UP:
    camera_->move_by(0, -1);
    break;
  case SDLK_DOWN:
    camera_->move_by(0, 1);
    break;
  }
}

void Viewport::handle_mouse_button_down(const SDL_MouseButtonEvent &e) {
  camera_->realm_->event_queue_.push_back(
      std::move(Event::create("mouse_button_down",
                              e.button,
                              e.x + camera_->rect_.x,
                              e.y + camera_->rect_.y)));
}

void Viewport::handle_mouse_motion (const SDL_MouseMotionEvent &e) {
  camera_->realm_->event_queue_.push_back(
      std::move(Event::create("mouse_motion",
                                    e.x + camera_->rect_.x,
                                    e.y + camera_->rect_.y,
                                    e.xrel, e.yrel)));
}
