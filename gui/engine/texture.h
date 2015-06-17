#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture {
public:
  Texture() = default;

  Texture(SDL_Texture* texture) {
    set(texture);
  }

  virtual ~Texture() {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
  }

  bool load(SDL_Renderer *renderer, const std::string &filename);

  SDL_Texture* get() const {
    return texture_;
  }

  void set(SDL_Texture* texture) {
    if (texture_)
      SDL_DestroyTexture(texture_);
    texture_ = texture;
  }

private:
  SDL_Texture* texture_ = nullptr;
};

#endif /* TEXTURE_H_ */
