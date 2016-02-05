#ifndef FRUITCANDY_CORE_CAMERA_H_
#define FRUITCANDY_CORE_CAMERA_H_

#include <string>
#include <memory>
#include <list>

#include <SDL2/SDL.h>

#include "realm.h"
#include "renderable.h"
#include "event_handler.h"

class Camera {
public:
  Camera(const SDL_Rect& rect, Realm *realm,
         const std::vector<std::string> &tags,
         std::unique_ptr<EventHandler> event_handler = nullptr)
      : rect_(rect), realm_(realm), tags_(tags),
        event_handler_(std::move(event_handler)) {}

  virtual ~Camera() = default;

public:
  SDL_Rect rect_ {0, 0, 0, 0};
  std::list<Renderable*> renderables_;
  Realm *realm_;
  const std::vector<std::string> tags_;
  std::unique_ptr<EventHandler> event_handler_ = nullptr;

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

  void shoot() {
    renderables_.clear();
    for (auto tag: tags_) {
      realm_->repo_->traverse<Object>(
          tag, [&](Object *object) {
            for (auto &component: object->components_) {
              if (dynamic_cast<Renderable*>(component.get())) {
                Renderable *r = dynamic_cast<Renderable*>(component.get());
                auto rect = r->get_absolute_rect();
                if (r->visible_ && SDL_HasIntersection(&rect, &rect_)) {
                  renderables_.push_back(r);
                }
              }
            }
      });
    }
  }

  void dispatch_event(const SDL_Event &e) {
    switch (e.type) {
    case SDL_KEYDOWN:
      handle_key_down(e.key);
      break;
    case SDL_KEYUP:
      handle_key_up(e.key);
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

protected:
  virtual void handle_key_down(const SDL_KeyboardEvent &e) {
    if (event_handler_) {
      event_handler_->handle_key_down(e.keysym.sym, e.repeat);
    }
  }

  virtual void handle_key_up(const SDL_KeyboardEvent &e) {
    if (event_handler_) {
      event_handler_->handle_key_up(e.keysym.sym);
    }
  }

  virtual void handle_mouse_button_down(const SDL_MouseButtonEvent &e) {
    if (event_handler_) {
        event_handler_->handle_mouse_button_down(
            e.button, e.x + rect_.x, e.y + rect_.y);
    }
  }

  virtual void handle_mouse_motion (const SDL_MouseMotionEvent &e) {
    if (event_handler_) {
      event_handler_->handle_mouse_motion(
          e.x + rect_.x, e.y + rect_.y, e.xrel, e.yrel);
    }
  }
};

#endif /* FRUITCANDY_CORE_CAMERA_H_ */
