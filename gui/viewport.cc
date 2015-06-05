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
    handle_key_down(e.key.keysym.sym);
    break;
  case SDL_MOUSEBUTTONDOWN:
    handle_mouse_button_down(e.button.button, {e.button.x, e.button.y});
    break;
  }
}

void Viewport::handle_key_down(SDL_Keycode sym) {
  camera_->realm_->event_queue_.push_back(
      std::move(Event::create("key_down", sym)));

  switch (sym) {
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

void Viewport::handle_mouse_button_down(Uint8 button, const SDL_Point& point) {
  camera_->realm_->event_queue_.push_back(
      std::move(Event::create("mouse_button_down",
                              point.x + camera_->rect_.x,
                              point.y + camera_->rect_.y)));
}
