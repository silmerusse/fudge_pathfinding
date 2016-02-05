#ifndef FRUITCANDY_RENDERABLE_SPRITE_H_
#define FRUITCANDY_RENDERABLE_SPRITE_H_

#include <string>
#include <sstream>
#include <memory>

#include "../core/renderable.h"
#include "../core/texture.h"

class Sprite : public Renderable {
public:
  Sprite(Graphics *graphics, int w, int h, int texture_id,
        const SDL_Rect *clip_rect = nullptr,  int z_order = 0)
      : Renderable(graphics, w, h, z_order) {
    set_texture(graphics, texture_id, clip_rect);
  }

  Sprite(const Sprite &sprite)
    : Renderable(sprite), 
      texture_(sprite.texture_),
      clip_rect_(sprite.clip_rect_ == nullptr ? nullptr 
          : std::unique_ptr<const SDL_Rect>(
              new SDL_Rect(*(sprite.clip_rect_)))) {};

  virtual ~Sprite() = default;

public:
  Texture *texture_ = nullptr;
  std::unique_ptr<const SDL_Rect> clip_rect_ = nullptr;

public:
  void set_texture(Graphics *graphics, int texture_id,
                   const SDL_Rect *clip_rect = nullptr) {
    if (graphics) {
      Texture *texture = graphics->assets_->get_item<Texture>(texture_id);
      if (texture) {
        texture_ = texture;
        if (clip_rect) {
          clip_rect_ = std::move(std::unique_ptr<const SDL_Rect>(
              new SDL_Rect(*clip_rect)));
        } else {
          clip_rect_ = nullptr;
        }
      }
    }
  }

  void render(const SDL_Rect &rect) const override {
    if (graphics_ && texture_) {
      graphics_->render_texture(texture_, &rect, clip_rect_.get(),
                                get_absolute_rotation());

      if (parent_ != nullptr && parent_->selected_) {
        SDL_RenderDrawRect(graphics_->renderer_, &rect);
      }
    } else {
      ERROR ("Sprite: Graphics or Texture not found.");
    }
  }


public:
  // Factory method to create a new instance.
  static std::unique_ptr<Sprite> create(
      Graphics *graphics, int w, int h, int texture_id,
      const SDL_Rect *clip_rect, int z_order = 0) {
    return std::unique_ptr<Sprite>(
        new Sprite(graphics, w, h, texture_id, clip_rect, z_order));
  }
};

#endif /* FRUITCANDY_RENDERABLE_SPRITE_H_ */
