#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "panel.h"

class Viewport: public Panel {
public:
  Viewport(const SDL_Rect& rect, int z_order = 0);
  virtual ~Viewport() = default;

public:
  void move(int x, int y) {
    rect_.x = x;
    rect_.y = y;
  }

  void scale(double s) {
    rect_.x = static_cast<int>(rect_.x - rect_.w / 2) * s + rect_.w / 2;
    rect_.y = static_cast<int>(rect_.y - rect_.h / 2) * s + rect_.h / 2;
    rect_.w = static_cast<int>(rect_.w * s);
    rect_.h = static_cast<int>(rect_.h * s);
  }

  void observe(std::unique_ptr<Camera> &&camera);

  virtual void render(SDL_Renderer *renderer) const override;

public:
  void handle_event(const SDL_Event &e) override;

public:
  std::unique_ptr<Camera> camera_ = nullptr;

private:
  void handle_key_down(const SDL_KeyboardEvent &e) override;
  void handle_mouse_button_down(const SDL_MouseButtonEvent &e) override;
  void handle_mouse_motion (const SDL_MouseMotionEvent &e) override;
};

#endif /* VIEWPORT_H_ */
