#ifndef FRUITCANDY_ENGINE_GRAPHICS_H_
#define FRUITCANDY_ENGINE_GRAPHICS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../util/log.h"
#include "repository.h"
#include "texture.h"

class Graphics {
public:
  Graphics(SDL_Renderer *renderer)
      : renderer_(renderer), assets_(new Repository()) {};
  virtual ~Graphics() {
    if (renderer_) {
      SDL_DestroyRenderer(renderer_);
      renderer_ = NULL;
    }
  }

public:
  // Transform drawing coordinates and set drawing area.
  void set_viewport(const SDL_Rect &rect) {
    SDL_RenderSetViewport(renderer_, &rect);
  }

  void save_viewport() {
    SDL_RenderGetViewport(renderer_, &saved_viewport_rect_);
  }

  void restore_viewport() {
    SDL_RenderSetViewport(renderer_, &saved_viewport_rect_);
  }

  void render_texture(const Texture *texture,  const SDL_Rect *rect,
                      const SDL_Rect *src_rect = nullptr, double rotation = 0) {
    if (SDL_RenderCopyEx(renderer_, texture->get_texture_ptr(), src_rect, rect,
                         rotation, nullptr, SDL_FLIP_NONE)) {
      ERROR("Render failed.");
    }
  }

public:
  SDL_Renderer *renderer_ = nullptr;
  std::unique_ptr<Repository> assets_ = nullptr;
  SDL_Rect saved_viewport_rect_;
};

#endif /* FRUITCANDY_ENGINE_GRAPHICS_H_ */
