#ifndef FRUITCANDY_CORE_TEXTURE_H_
#define FRUITCANDY_CORE_TEXTURE_H_

#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "asset.h"

class Texture : public Asset {
public:
  Texture() {
    loader_ = TextureLoader::instance();
  }

  Texture(SDL_Texture *texture) {
    loader_ = TextureLoader::instance();
    set(texture);
  }

  virtual ~Texture() {
    if (texture_)
      SDL_DestroyTexture(texture_);
    texture_ = nullptr;
  }

  bool open(SDL_Renderer *renderer) {
    texture_ = IMG_LoadTexture(renderer, path_.c_str());
    return texture_ != nullptr;
  }

  SDL_Texture* get_texture_ptr() const {
    return texture_;
  }

  void set(SDL_Texture* texture) {
    if (texture_)
      SDL_DestroyTexture(texture_);
    texture_ = texture;
  }

  SDL_Texture* clone(SDL_Renderer *renderer) {
    if (texture_) {
      int w, h, access;
      Uint32 format;
      SDL_QueryTexture(texture_, &format, &access, &w, &h);
      SDL_Texture *texture = SDL_CreateTexture(
          renderer, format,
          SDL_TEXTUREACCESS_TARGET, w, h);
      SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
      SDL_SetRenderTarget(renderer, texture);
      SDL_RenderCopy(renderer, texture_, NULL, NULL);
      SDL_SetRenderTarget(renderer, NULL);
      return texture;
    } else
      return nullptr;
  }

private:
  SDL_Texture* texture_ = nullptr;

private:
  class TextureLoader : public Loader {
    public:
    TextureLoader() = default;
    virtual ~TextureLoader() = default;

  public:
    virtual void load(Item *item, Value &v) override {
      Texture *texture = static_cast<Texture*>(item);
      texture->name_ = v.get_string("name");
      texture->path_ = v.get_string("path");
    }

    static TextureLoader* instance() {
      static TextureLoader instance_;
      return &instance_;
    }
  };
};

#endif /* FRUITCANDY_CORE_TEXTURE_H_ */
