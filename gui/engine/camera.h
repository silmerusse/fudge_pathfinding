#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>
#include <memory>
#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "realm.h"
#include "renderable.h"

class Camera {
public:
  Camera(const SDL_Rect& rect, Realm *realm,
         const std::vector<std::string> &store_names_);
  virtual ~Camera() = default;

public:
  SDL_Rect rect_ {0, 0, 0, 0};
  std::list<Renderable*> renderables_;
  Realm *realm_;
  const std::vector<std::string> store_names_;

public:
  void move_by(int dx, int dy) {
    rect_.x += dx;
    rect_.y += dy;
  }

  void move_to(int x, int y) {
    rect_.x = x;
    rect_.y = y;
  }

  void scale(double s) {
    rect_.x = static_cast<int>(rect_.x - rect_.w / 2) * s + rect_.w / 2;
    rect_.y = static_cast<int>(rect_.y - rect_.h / 2) * s + rect_.h / 2;
    rect_.w = static_cast<int>(rect_.w*s);
    rect_.h = static_cast<int>(rect_.h*s);
  }

  void shoot();

  void dispatch_event(const SDL_Event &e);

private:
  void handle_key_down(const SDL_KeyboardEvent &e);
  void handle_mouse_button_down(const SDL_MouseButtonEvent &e);
  void handle_mouse_motion (const SDL_MouseMotionEvent &e);
};

#endif /* CAMERA_H_ */
