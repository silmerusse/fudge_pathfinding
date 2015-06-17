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

void Viewport::dispatch_event(const SDL_Event &e) {
  camera_->dispatch_event(e);
}
