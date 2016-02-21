#ifndef FRUITCANDY_ENGINE_RENDERABLE_H_
#define FRUITCANDY_ENGINE_RENDERABLE_H_

#include <string>
#include <memory>
#include <sstream>

#include "../math/vector2D.h"

#include "object.h"
#include "graphics.h"

class Renderable: public Object {
public:
  Renderable(Graphics *graphics, int w = 0, int h = 0, int z_order = 0)
      : w_(w), h_(h), z_order_(z_order), graphics_(graphics) {
    transform_.reset(new Transform());
  }

  Renderable(const Renderable &renderable) :
      w_(renderable.w_), h_(renderable.h_), z_order_(renderable.z_order_),
      visible_(renderable.visible_), graphics_(renderable.graphics_) {
    transform_.reset(new Transform());
  }

  virtual ~Renderable() = default;

public:
  virtual void render(const SDL_Rect &rect) const {}

  virtual const SDL_Rect get_rect() const {
      return SDL_Rect{static_cast<int>(transform_->get_position().x - w_ / 2),
                      static_cast<int>(transform_->get_position().y - h_ / 2),
                      w_, h_};
  }

  virtual const SDL_Rect get_absolute_rect() const {
    auto pos = get_absolute_position();
    return SDL_Rect{static_cast<int>(pos.x - w_ / 2),
                    static_cast<int>(pos.y - h_ / 2),
                    w_, h_};
  }

  virtual const SDL_Rect get_boundary_box() const {
    return {0, 0, w_, h_};
  }

  virtual const std::string to_string() const override {
    std::ostringstream ss;
    ss << '{' << transform_->get_position().x << ','
              << transform_->get_position().y << ','
              << w_ << ',' << h_ << '}' << ", ";
    return ss.str();
  }

  const SDL_Rect get_render_rect(const Vector2D<int> &offset) const {
    return SDL_Rect{
      static_cast<int>(get_absolute_position().x - w_ / 2 + offset.x),
      static_cast<int>(get_absolute_position().y - h_ / 2 + offset.y),
      w_, h_};
  }

public:
  int w_ = 0;
  int h_ = 0;
  int z_order_ = 0;
  bool visible_ = true;

protected:
  Graphics *graphics_ = nullptr;
};

#endif /* FRUITCANDY_ENGINE_RENDERABLE_H_ */
