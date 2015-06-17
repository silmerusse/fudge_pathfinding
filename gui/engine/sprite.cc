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
  SDL_Rect rect = get_render_rect(offset);
  render_texture(renderer, texture_, &rect, &clip_rect_);

  if (nullptr != parent_ && parent_->selected_) {
    SDL_RenderDrawRect(renderer, &rect);
  }
}

