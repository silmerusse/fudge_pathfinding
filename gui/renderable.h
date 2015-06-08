#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <string>
#include <memory>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "object.h"
#include "vector2D.h"

class Renderable: public Object {
public:
  Renderable(int z_order = 0) :
      Object(), z_order_(z_order) {}
  virtual ~Renderable() = default;

public:
  virtual void render(SDL_Renderer *renderer, const Vector2D<int> &offset) {}

  const SDL_Rect get_rect() const {
    return SDL_Rect{static_cast<int>(get_absolute_position().x - w_ / 2),
                    static_cast<int>(get_absolute_position().y - h_ / 2),
                    w_, h_};
  }

  virtual const std::string to_string() const override {
    std::ostringstream ss;
    ss << '{' << get_position().x << ',' << get_position().y << ','
              << w_ << ',' << h_ << '}' << ", ";
    return ss.str();
  }

public:
  int w_ = 0;
  int h_ = 0;
  int z_order_ = 0;
  bool visible_ = true;
};

#endif /* RENDERABLE_H_ */
