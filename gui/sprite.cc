#include "log.h"
#include "sprite.h"

Sprite::Sprite(const Texture *texture, int z_order) :
    Renderable(z_order) {
  set_texture(texture);
}

void Sprite::set_texture(const Texture *texture) {
  texture_ = texture;
  clip_rect_.x = clip_rect_.y = 0;
  SDL_QueryTexture(texture_->get(), NULL, NULL, &w_, &h_);
  clip_rect_.w = w_;
  clip_rect_.h = h_;
}

void Sprite::render(SDL_Renderer *renderer, const Vector2D<int> &offset) {
  SDL_Rect rect;
  rect.x = get_absolute_position().x - w_ / 2 + offset.x;
  rect.y = get_absolute_position().y - h_ / 2 + offset.y;
  rect.w = w_;
  rect.h = h_;

  if (SDL_RenderCopyEx(renderer, texture_->get(), &clip_rect_, &rect,
                       get_absolute_rotation(), nullptr, SDL_FLIP_NONE)) {
    ERROR("Render failed.");
  }

  if (parent_->selected_) {
    SDL_RenderDrawRect(renderer, &rect);
  }
}

