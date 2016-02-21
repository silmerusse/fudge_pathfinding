#ifndef FRUITCANDY_ENGINE_VIEWPORT_H_
#define FRUITCANDY_ENGINE_VIEWPORT_H_

#include <memory>
#include "camera.h"
#include "panel.h"

class Viewport: public Panel {
public:
  Viewport(Graphics *graphics, const SDL_Rect &rect, int z_order = 0)
      : Panel(graphics, rect, z_order) {}
  virtual ~Viewport() = default;

public:
  void move(int x, int y) {
    transform_->set_position({x, y});
  }

  void observe(std::unique_ptr<Camera> &&camera) {
    camera_ = std::move(camera);
  }

  virtual void render(const SDL_Rect &rect) const override {
    Panel::render(rect);

    if (camera_ && graphics_) {
      camera_->shoot();

      Vector2D<int> offset{-camera_->rect_.x, -camera_->rect_.y};
      // Draw each sprite visible in the camera.
      for (const auto &renderable : camera_->renderables_) {
        SDL_Rect render_rect = renderable->get_render_rect(offset);
        renderable->render(render_rect);
      }
    }
  }

public:
  void dispatch_event(const SDL_Event &e) override {
    if (camera_)
      camera_->dispatch_event(e);
  }

public:
  std::unique_ptr<Camera> camera_ = nullptr;
};

#endif /* FRUITCANDY_ENGINE_VIEWPORT_H_ */
