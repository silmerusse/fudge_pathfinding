#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <sstream>
#include <memory>
#include "renderable.h"
#include "texture.h"

class Sprite : public Renderable {
public:
  explicit Sprite(const Texture *texture, int z_order = 0);
  explicit Sprite(Sprite *s) {
    texture_ = s->texture_;
    clip_rect_ = s->clip_rect_;
    z_order_ = s->z_order_;
    w_ = s->w_;
    h_ = s->h_;
  }
  virtual ~Sprite() = default;

public:
  const Texture *texture_;
  SDL_Rect clip_rect_ {0, 0, 0, 0}; // Clip rect on the texture.

public:
  void set_texture(const Texture *texture);
  virtual void render(SDL_Renderer *renderer,
                      const Vector2D<int> &offset) override;
  std::unique_ptr<Sprite> clone() {
    return std::unique_ptr<Sprite>(new Sprite(this));
  }

public:
  // Factory method to create a new instance.
  static std::unique_ptr<Sprite> create(const Texture *texture,
                                        int z_order = 0) {
    return std::unique_ptr<Sprite>(new Sprite(texture, z_order));
  }
};

#endif /* SPRITE_H_ */
